#include <stdio.h>
#include "bike-computer.h"
#include "blue.h"
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
void blue_init(void);

void app_main(void)
{
    blue_init();

    // weather_task_start();
    gps_task_start();

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