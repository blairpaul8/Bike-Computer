#include "bike-computer.h"
//#include "Bluetooth/blue.h"
#include "drivers/ble/BLE.hpp"
#include "HeartRate/IHeartRate.hpp"
#include "HeartRate/HeartRateMonitor.hpp"

#include "esp_log.h"
#include "nvs_flash.h"

#include <stdio.h>

extern "C" void app_main(void)
{
    static BleDriver ble;               // static: must outlive app_main,
    static HeartRateMonitor hrm(ble);   // the BLE host task keeps using them

    if (!hrm.init())
    {
        ESP_LOGE("main", "HeartRateMonitor init failed");
        return;
    }

    HeartRateSample s;
    while (true)
    {
        if (hrm.read(s, 5000))
        {
            ESP_LOGI("main", "HR: %u bpm", s.bpm);
        }
        else
        {
            ESP_LOGW("main", "No HR sample in 5 s");
        }
    }

    //blue_init();
    // gps_task_start();
    //display_task_start();
}