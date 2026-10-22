#include <stdio.h>
#include "bike-computer.h"

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

// F2:98:C8:AB:67:57 -- heart rate strap name
static const uint8_t TARGET_ADDR[6] = {0x57, 0x67, 0xAB, 0xC8, 0x98, 0xF2};

void app_main(void)
{
    // weather_task_start();
    gps_task_start();
}