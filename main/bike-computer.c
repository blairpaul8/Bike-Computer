#include "bike-computer.h"
#include "Bluetooth/blue.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include <stdio.h>

void app_main(void)
{
    BleDriver hr= new BleDriver();
    hr.begin(0x180D, 0x2a37, nullptr, nullptr);
    blue_init();
    // gps_task_start();
    display_task_start();
}