// ble_driver.cpp
#include "ble_driver.hpp"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"
#include "esp_central.h"

static const char *TAG = "BleDriver";

// shared state (was: s_ownAddrType, plus the HR #defines)
static uint16_t s_serviceUuid;
static uint16_t s_charUuid;
static BleNotifyCallback s_notifyCb;
static void *s_notifyCtx;
static uint8_t s_ownAddrType;
static bool s_connected;

// forward declarations (they call each other)
static void scan(void);
static void onDiscComplete(const struct peer *peer, int status, void *arg);

// the old functions, moved here as-is
static bool shouldConnect(const struct ble_gap_disc_desc *disc)
{
    // was blecent_should_connect: compare against s_serviceUuid instead of s_serviceUuid
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
        if (ble_uuid_u16(&fields.uuids16[i].u) == s_serviceUuid)
        {
            return 1;
        }
    }

    return 0;
}

static int gapEvent(struct ble_gap_event *event, void *arg)
{
    // was gapEvent:
    //  - NOTIFY_RX: copy the mbuf, then call s_notifyCb(buf, len, s_notifyCtx). No HR parsing.
    //  - CONNECT: set s_connected = true on success
    //  - DISCONNECT: set s_connected = false

    switch (event->type)
    {

    // a device was seen
    case BLE_GAP_EVENT_DISC:
        if (shouldConnect(&event->disc))
        {
            ble_gap_disc_cancel();
            ble_gap_connect(s_ownAddrType, &event->disc.addr, 30000, NULL,
                            gapEvent, NULL);
        }
        return 0;
    // a connection attempt finished
    case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0)
        {
            peer_add(event->connect.conn_handle);
            peer_disc_all(event->connect.conn_handle, onDiscComplete, NULL);
        }
        else
        {
            scan();
        }
        return 0;
    // link dropped
    case BLE_GAP_EVENT_DISCONNECT:
        peer_delete(event->disconnect.conn.conn_handle);
        scan();
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

static void onDiscComplete(const struct peer *peer, int status, void *arg)
{
    // was onDiscComplete: use s_serviceUuid / s_charUuid
    // If discovery failed, disconnect. That error code means “terminated by the local user”.
    if (status != 0)
    {
        ble_gap_terminate(peer->conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        return;
    }

    // Looks through the discovered data for a descriptor by walking: service 0x180D
    const struct peer_dsc *dsc = peer_dsc_find_uuid(peer, BLE_UUID16_DECLARE(s_serviceUuid), BLE_UUID16_DECLARE(s_charUuid), BLE_UUID16_DECLARE(BLE_GATT_DSC_CLT_CFG_UUID16));

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

static void scan(void)
{
    // was scan: use s_ownAddrType
    // {0} zeroes every field first.
    struct ble_gap_disc_params disc_params = {0};

    // means you get a report every time a device advertises,
    // not just the first time.
    disc_params.filter_duplicates = 0;

    // means active scanning, which also asks devices for
    // their scan response (where names often live)
    disc_params.passive = 0;

    int rc = ble_gap_disc(s_ownAddrType, BLE_HS_FOREVER, &disc_params, gapEvent, NULL);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gap_disc failed: %d", rc);
    }
    else
    {
        ESP_LOGI(TAG, "Scanning for 10 s...");
    }
}

static void onSync(void)
{
    // was blecent_on_sync: infer address into s_ownAddrType, then scan()
    // ensure_addr makes sure the device has a Bluetooth 
    // address to use.
    ble_hs_util_ensure_addr(0);

    // picks the best address type available 
    // (public or random) and stores it in
    ble_hs_id_infer_auto(0, &s_ownAddrType);

    scan();
}

static void hostTask(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

// ---- the public API from the header ----
bool BleDriver::begin(uint16_t serviceUuid, uint16_t charUuid, BleNotifyCallback cb, void *ctx)
{
    s_serviceUuid = serviceUuid;
    s_charUuid = charUuid;
    s_notifyCb = cb;
    s_notifyCtx = ctx;

    // was blue_init()
    nvs_flash_init();
    nimble_port_init();
    ble_hs_cfg.sync_cb = onSync;
    peer_init(MYNEWT_VAL(BLE_MAX_CONNECTIONS), 64, 64, 64);
    nimble_port_freertos_init(hostTask);
    return true;
}

bool BleDriver::isConnected() const
{
    return s_connected;
}