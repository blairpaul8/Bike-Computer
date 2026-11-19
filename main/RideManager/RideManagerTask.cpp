// RideManagerTask.cpp

#include "RideManagerTask.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queque.h"

#include <cstdint>
#include <memory>

RideManagerTask::RideManagerTask(QueueHandle_t hr_queue, QueueHandle_t gps_queue)
    : hr_client_queue_(hr_queue), gps_client_queue_(gps_queue) {}

bool RideManagerTask::init()
{
    // TODO:
    return true;
}

void HeartRateTask::run()
{

    while (true)
    {
        ESP_LOGI(kTag, "Ride Manger Running");
    }
}

extern "C" void HeartRate_task_start(QueueHandle_t hr_queue, QueueHandle_t gps_queue)
{
    static RideManagerTask ride_manager_task(hr_queue, gps_queue);
    RideManagerTask.start("RideManagerTask");
}