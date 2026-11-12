#include "bike-computer.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include <stdio.h>

void app_main(void)
{
    HeartRate_task_start();
    //blue_init();
    // gps_task_start();
    //display_task_start();
}