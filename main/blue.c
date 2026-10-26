#include <stdio.h>
#include "blue.h"
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
#include "esp_central.h"

#define BLECENT_SVC_HR_UUID 0x180D
#define BLECENT_CHR_HR_MEAS 0x2A37
static const char *TAG = "hr_scan";

static uint8_t own_addr_type;



static int blecent_gap_event(struct ble_gap_event *event, void *arg)
{
    // Advertisements come in types. These two are the connectable ones, so anything else 
    // (like broadcast-only beacons) is rejected immediately.
    if (disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_ADV_IND && disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_DIR_IND)
    {
        return 0;
    }
    struct ble_hs_adv_fields fields;
    
    // An advertisement is a packed byte blob. This unpacks it into a ble_hs_adv_fields struct with the name, the UUID lists, 
    // and so on. It returns nonzero if the data is malformed.
    if (ble_hs_adv_parse_fields(&fields, disc->data, disc->length_data) != 0)
    {
        return 0;
    }

    // Loops over the 16-bit service UUIDs the device advertises, and returns 1 if one is 0x180D (Heart Rate). 
    // That’s how it recognizes a heart rate strap.
    for (int i = 0; i < fields.num_uuids16; i++)
    {
        if (ble_uuid_u16(&fields.uuids16[i].u) == BLECENT_SVC_HR_UUID)
        {
            return 1;
        }
    }

    return 0;
}

// This is the center of the program. NimBLE calls it for every GAP-level event, 
// and the switch picks the case by event->type. Returning 0 means “handled”.
static int blecent_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {

    // a device was seen
    case BLE_GAP_EVENT_DISC:
        if (blecent_should_connect(&event->disc))
        {
            ble_gap_disc_cancel();
            ble_gap_connect(own_addr_type, &event->disc.addr, 30000, NULL,
                            blecent_gap_event, NULL);
        }
        return 0;
    // a connection attempt finished
    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0)
        {
            peer_add(event->connect.conn_handle);
            peer_disc_all(event->connect.conn_handle, blecent_on_disc_complete, NULL);
        }
        else
        {
            blecent_scan();
        }
        return 0;
    // link dropped
    case BLE_GAP_EVENT_DISCONNECT:
        peer_delete(event->disconnect.conn.conn_handle);
        blecent_scan();
        return 0;
    // data arrived from the HR monitor
    case BLE_GAP_EVENT_NOTIFY_RX:
    {
        uint8_t buf[4] = {0};
        uint16_t len = OS_MBUF_PKTLEN(event->notify_rx.om);
        if (len > sizeof(buf))
            len = sizeof(buf);
        os_mbuf_copydata(event->notify_rx.om, 0, len, buf);

        // buf[0] is the flags byte. Bit 0 set = 16-bit value (buf[1] low, buf[2] high);
        // bit 0 clear = 8-bit value in buf[1]
        uint16_t hr = (buf[0] & 0x01) ? (buf[1] | (buf[2] << 8)) : buf[1];
        ESP_LOGI(TAG, "Heart rate: %u bpm", hr);
        return 0;
    }

    default:
        return 0;
    }
}

// DISC is short for “discovery.” In BLE_GAP_EVENT_DISC, 
// it’s the event NimBLE fires every time your scanner receives an advertisement from a nearby device.
static void blecent_on_disc_complete(const struct peer *peer, int status, void *arg)
{

    // If discovery failed, disconnect. That error code means “terminated by the local user”.
    if (status != 0)
    {
        ble_gap_terminate(peer->conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        return;
    }

    // Looks through the discovered data for a descriptor by walking: service 0x180D
    const struct peer_dsc *dsc = peer_dsc_find_uuid(peer, BLE_UUID16_DECLARE(BLECENT_SVC_HR_UUID), BLE_UUID16_DECLARE(BLECENT_CHR_HR_MEAS), BLE_UUID16_DECLARE(BLE_GATT_DSC_CLT_CFG_UUID16));

    if (dsc == NULL)
    {
        ESP_LOGE(TAG, "HR CCCD couldn't be found");
        ble_gap_terminate(peer->conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        return;
    }

    
    uint8_t value[2] = {1, 0};

    // This is the subscribe. The CCCD is a little switch on the strap: writing 0x0001 (low byte first, so {1, 0}) says “send me notifications”. After this write, the strap starts pushing heart 
    // rate data, which arrives in the NOTIFY_RX case above.
    int rc = ble_gattc_write_flat(peer->conn_handle, dsc->dsc.handle, value, sizeof(value), NULL, NULL);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "Subscribe failed; rc=%d", rc);
    }
}

static void blecent_scan(void)
{
    // {0} zeroes every field first.
    struct ble_gap_disc_params disc_params = {0};

    // means you get a report every time a device advertises, 
    // not just the first time.
    disc_params.filter_duplicates = 0;

    // means active scanning, which also asks devices for 
    // their scan response (where names often live)
    disc_params.passive = 0;

    int rc = ble_gap_disc(own_addr_type, BLE_HS_FOREVER, &disc_params, blecent_gap_event, NULL);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gap_disc failed: %d", rc);
    }
    else
    {
        ESP_LOGI(TAG, "Scanning for 10 s...");
    }
}


static void blecent_on_sync(void)
{
    // ensure_addr makes sure the device has a Bluetooth 
    // address to use.
    ble_hs_util_ensure_addr(0);

    // picks the best address type available 
    // (public or random) and stores it in
    ble_hs_id_infer_auto(0, &own_addr_type);

    blecent_scan();
}

static void host_task(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void app_main(void)
{
    // NimBLE stores some state (like bonding keys) in flash through NVS, so it has to be initialized first.
    nvs_flash_init();

    // Starts the Bluetooth controller and the NimBLE host stack.
    nimble_port_init();

    // Registers a callback. The host takes a moment to sync with the controller, and 
    //when it’s ready it calls blecent_on_sync. You must not start scanning before that, 
    //which is why scanning begins there and not in app_main.
    ble_hs_cfg.sync_cb = blecent_on_sync;

    // Sets up the peer helper (from the blecent example’s peer.c). A “peer” is a record 
    // of a connected device and everything discovered on it. The four numbers are the maximum 
    // counts of peers, services, characteristics, and descriptors it can store, which 
    // it allocates up front from memory pools.
    peer_init(MYNEWT_VAL(BLE_MAX_CONNECTIONS), 64, 64, 64);

    // Creates the FreeRTOS task that runs the BLE host. host_task calls nimble_port_run(), 
    // which loops forever processing BLE events and dispatching your callbacks. The nimble_port_freertos_deinit() 
    // after it only runs if the host is shut down.
    nimble_port_freertos_init(host_task);
}
