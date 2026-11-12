#include "HeartRateTask.hpp"
#include "../drivers/ble/BLE.hpp"
#include "HeartRateMonitor.hpp"
#include <cstdint>
#include "../HeartRate/IHeartRate.hpp"
#include "esp_log.h"
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

bool HeartRateTask::init()
{
    HeartRate_ble_ = std::make_unique<BleDriver>();
    HeartRate_ = std::make_unique<HeartRateMonitor>(*HeartRate_ble_);
    return HeartRate_->init();
}

void HeartRateTask::run()
{
    HeartRateSample s;

    while (true)
    {
        if (HeartRate_->read(s, 5000))
        {
            ESP_LOGI("main", "HR: %u bpm", s.bpm);
        }
        else
        {
            ESP_LOGW("main", "No HR sample in 5 s");
        }
    }
}

extern "C" void HeartRate_task_start()
{
    static HeartRateTask HeartRateTask;
    HeartRateTask.start("HeartRateTask");
}