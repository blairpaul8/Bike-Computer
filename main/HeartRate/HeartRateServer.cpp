// HeartRateServer.cpp
#include "HeartRateServer.hpp"

bool HeartRateServer::init()
{
    mailbox_ = xQueueCreate(1, sizeof(HeartRateSample));
    if (mailbox_ == nullptr)
    {
        return false;
    }
    return true;
}

void HeartRateServer::publish(HeartRateSample &sample)
{
    // Note: If we change the queue to be more than size 1
    // we need to change this to xQueueSend()
    xQueueOverwrite(mailbox_, &sample); // never blocks
}