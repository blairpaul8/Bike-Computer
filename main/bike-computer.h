#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

void HeartRate_task_start(QueueHandle_t queue);
void gps_task_start(void);
void display_task_start(void);;

#ifdef __cplusplus
}
#endif