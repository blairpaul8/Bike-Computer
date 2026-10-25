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
    if (disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_ADV_IND && disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_DIR_IND)
    {
        return 0;
    }
    struct ble_hs_adv_fields fields;

    if (ble_hs_adv_parse_fields(&fields, disc->data, disc->length_data) != 0)
    {
        return 0;
    }

    for (int i = 0; i < fields.num_uuids16; i++)
    {
        if (ble_uuid_u16(&fields.uuids16[i].u) == BLECENT_SVC_HR_UUID)
        {
            return 1;
        }
    }

    return 0;
}

static int blecent_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {

    case BLE_GAP_EVENT_DISC:
        if (blecent_should_connect(&event->disc))
        {
            ble_gap_disc_cancel();
            ble_gap_connect(own_addr_type, &event->disc.addr, 30000, NULL,
                            blecent_gap_event, NULL);
        }
        return 0;

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

    case BLE_GAP_EVENT_DISCONNECT:
        peer_delete(event->disconnect.conn.conn_handle);
        blecent_scan();
        return 0;

    case BLE_GAP_EVENT_NOTIFY_RX:
    {
        uint8_t buf[4] = {0};
        uint16_t len = OS_MBUF_PKTLEN(event->notify_rx.om);
        if (len > sizeof(buf))
            len = sizeof(buf);
        os_mbuf_copydata(event->notify_rx.om, 0, len, buf);
        uint16_t hr = (buf[0] & 0x01) ? (buf[1] | (buf[2] << 8)) : buf[1];
        ESP_LOGI(TAG, "Heart rate: %u bpm", hr);
        return 0;
    }

    default:
        return 0;
    }
}

static int blecent_on_subscribe(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg)
{
    ESP_LOGI(TAG, "Subscribe status=%d", error->status);
    return 0;
}

static void
blecent_on_disc_complete(const struct peer *peer, int status, void *arg)
{

    if (status != 0)
    {
        ble_gap_terminate(peer->conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        return;
    }

    const struct peer_dsc *dsc = peer_dsc_find_uuid(peer, BLE_UUID16_DECLARE(BLECENT_SVC_HR_UUID), BLE_UUID16_DECLARE(BLECENT_CHR_HR_MEAS), BLE_UUID16_DECLARE(BLE_GATT_DSC_CLT_CFG_UUID16));

    if (dsc == NULL)
    {
        ESP_LOGE(TAG, "HR CCCD couldn't be found");
        ble_gap_terminate(peer->conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        return;
    }

    uint8_t value[2] = {1, 0};
    int rc = ble_gattc_write_flat(peer->conn_handle, dsc->dsc.handle, value, sizeof(value), NULL, NULL);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "Subscribe failed; rc=%d", rc);
    }
}

static void blecent_scan(void)
{
    struct ble_gap_disc_params disc_params = {0};
    disc_params.filter_duplicates = 0;
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
    ble_hs_util_ensure_addr(0);
    ble_hs_id_infer_auto(0, &own_addr_type);
    blecent_scan();
}

static void host_task(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

