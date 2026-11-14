// HeartRateServer.cpp
#include "HeartRateServer.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


HeartRateServer::HeartRateServer(QueueHandle_t queue)
    : mailbox_(queue) {}

void HeartRateServer::publish(HeartRateSample &sample)
{
    // Note: If we change the queue to be more than size 1
    // we need to change this to xQueueSend()
    xQueueOverwrite(mailbox_, &sample); // never blocks
}