// HeartRateClient.hpp
#pragma once

#include "IHeartRate.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class HeartRateClient
{
public:
    // Constructor
    HeartRateClient(QueueHandle_t mailbox) : mailbox_(mailbox) {}

    // returns true if heart rate was written to out parameter
    bool getHeartRate(HeartRateSample &out) const
    {
        return mailbox_ != nullptr && 
                           xQueuePeek(mailbox_, &out, 0) == pdTRUE;
    }

private:

    // Message queue for transmitting HeartRateSamples
    QueueHandle_t mailbox_;
};