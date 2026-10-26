#include "bike-computer.h"
#include "Bluetooth/blue.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include <stdio.h>



void app_main(void)
{
    blue_init();

    // weather_task_start();
    gps_task_start();

}