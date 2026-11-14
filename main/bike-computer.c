#include "bike-computer.h"

#include "HeartRate/HeartRateReading.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include <stdio.h>

void app_main(void)
{

    // Create all message queue handles here so the handle for the server and client can
    // be passed to their cooresponding task.
    // EX. The HeartRateServer and Client both need the same handle so the
    //     hr_queue can be passed to the hr task for the server. Then the
    //     same handle can be passed to the RideManager so it has the
    //     "other end" of the queue.
    QueueHandle_t hr_queue = xQueueCreate(1, sizeof(HeartRateSample));



    // Start all tasks.
    HeartRate_task_start(hr_queue);
    // gps_task_start();
    //display_task_start();
}