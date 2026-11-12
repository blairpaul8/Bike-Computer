#include "HeartRateTask.hpp"

#include "../drivers/ble/BLE.hpp"
#include "HeartRateMonitor.hpp"
#include "../HeartRate/IHeartRate.hpp"
#include "HeartRateServer.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cstdint>
#include <memory>

bool HeartRateTask::init()
{
    HeartRate_ble_ = std::make_unique<BleDriver>();

    HeartRate_ = std::make_unique<HeartRateMonitor>(*HeartRate_ble_);

    hr_server_ = std::make_unique<HeartRateServer>();

    if (!HeartRate_->init()) {
        // TODO: Log
        return false;
    }

    if (!hr_server_->init()) {
        // TODO: Log
        return false;
    }

    return true;
}

void HeartRateTask::run()
{
    HeartRateSample hr_sample;

    while (true)
    {
        if (HeartRate_->read(hr_sample, 5000))
        {
            ESP_LOGI("main", "HR: %u bpm", hr_sample.bpm);
            hr_server_->publish(hr_sample);
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