// RideManagerTask.cpp
#pragma once

#include <memory>
#include "../tasks/task.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class RideManagerTask : public Task<RideManagerTask> {
public:
    // ctor
    RideManagerTask(QueueHandle_t hr_queue, QueueHandle_t gps_queue);

    bool init();

    void run();

private:
    // Members are destroyed in reverse declaration order.
    // The HeartRate must be destroyed before its SPI bus.

    // add smart pointer to clients?

    QueueHandle_t hr_client_queue_;

    QueueHandle_t gps_client_queue_;

    static constexpr char[] kTag = "RideManager"

};

extern "C" void HeartRate_task_start(QueueHandle_t hr_queue, QueueHandle_t gps_queue);