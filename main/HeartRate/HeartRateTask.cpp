#include "HeartRateTask.hpp"
#include "../drivers/ble/BLE.hpp"
#include "HeartRateMonitor.hpp"
#include <cstdint>
#include "../HeartRate/IHeartRate.hpp"
#include "esp_log.h"
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include <cstdint>
#include <memory>

HeartRateTask::HeartRateTask(QueueHandle_t queue)
    : server_queue_(queue) {}

bool HeartRateTask::init()
{
    HeartRate_ble_ = std::make_unique<BleDriver>();
    HeartRate_ = std::make_unique<HeartRateMonitor>(*HeartRate_ble_);

    hr_server_ = std::make_unique<HeartRateServer>(server_queue_);

    if (!HeartRate_->init()) {
        // TODO: Log
        return false;
    }

    return true;
}

void HeartRateTask::run()
{
    HeartRateSample s;

    while (true)
    {
        // read HR sample
        if (HeartRate_->read(hr_sample, 5000))
        {
            ESP_LOGI("main", "HR: %u bpm", hr_sample.bpm);
            // put on the queue for the client
            hr_server_->publish(hr_sample);
        }
        else
        {
            ESP_LOGW("main", "No HR sample in 5 s");
        }
    }
}

extern "C" void HeartRate_task_start(QueueHandle_t queue)
{
    static HeartRateTask hr_task(queue);
    hr_task.start("HeartRateTask");
}