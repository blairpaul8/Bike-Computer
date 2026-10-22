
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
/* BLE */
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
#include "blecent.h"

static const uint8_t TARGET_ADDR[6] = {0x57, 0x67, 0xAB, 0xC8, 0x98, 0xF2};

static int blecent_should_connect(const struct ble_gap_disc_desc *disc) // scan and filter for bluetooth
{
    /* The device has to be advertising connectability. */
    if (disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_ADV_IND && disc->event_type != BLE_HCI_ADV_RPT_EVTYPE_DIR_IND)
    {
        return 0;
    }
    struct ble_hs_adv_fields fields;

    if (ble_hs_adv_fields(&fields, disc->data, disc->length_data) != 0) // filling the field struct with the values found.
    {
        return 0;
    }

    for (int i = 0; i < fields.num_uuids16; i++)
    {
        if (ble_uuid_u16(&fields.uuids16[i].u) == 0x180D)
        { // address of heart rate monitors
            return 1;
        }
    }

    return 0;
}
//tells whether we got a subscriptions or not. 
static int blecent_on_subscribe(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg)
{
    ESP_LOGI(tag, "Subscribe status=%d", error->status);
    return 0;
}


/**
 * Called when service discovery of the specified peer has completed.
 */
static void
blecent_on_disc_complete(const struct peer *peer, int status, void *arg)
{

    if (status != 0) {
        /* Service discovery failed.  Terminate the connection. */
        ble_gap_terminate(peer->conn_handle, BLE_ERR_REM_USER_CONN_TERM);
        return;
    }

    //this needs to find the CCCD for the heart rate monitor and write 0x0001 to get notifications 
    //from the actual heart rate monitor. 



    //! need to add the gap event part that actually parses for the heart rate information. 
}