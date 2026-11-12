// HeartRateServer.hpp
#pragma once

#include "IHeartRate.hpp"
#include "HeartRateClient.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class HeartRateServer
{
public:
    // Default constructor
    HeartRateServer() = default;

    // Initializes the queue.
    bool init();

    // Puts the heart rate reading passed in onto the
    // queue
    void publish(HeartRateSample &sample);

    // Returns a HeartRateClient but doesn't own it.
    HeartRateClient client() const { return HeartRateClient(mailbox_); }


private:

    // Queue used to send HeartRate readings to the client
    QueueHandle_t mailbox_ = nullptr;
};