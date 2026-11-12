// HeartRateMonitor.cpp
#include "HeartRateMonitor.hpp"

bool HeartRateMonitor::init()
{
    // Queue of ONE sample. The BLE side overwrites it,
    // so read() always gets the latest value.
    queue_ = xQueueCreate(1, sizeof(HeartRateSample));
    if (queue_ == nullptr)
    {
        return false;
    }
    return ble_.begin(kServiceUuid, kCharUuid, &HeartRateMonitor::notifyThunk, this);
}

bool HeartRateMonitor::read(HeartRateSample &out, uint32_t timeoutMs)
{
    return xQueueReceive(queue_, &out, pdMS_TO_TICKS(timeoutMs)) == pdTRUE;
}

void HeartRateMonitor::notifyThunk(const uint8_t *data, size_t len, void *ctx)
{
    static_cast<HeartRateMonitor *>(ctx)->onNotify(data, len);
}

// Runs on the NimBLE host task: parse, drop in the queue, return. Never block.
void HeartRateMonitor::onNotify(const uint8_t *data, size_t len)
{
     // Need at least the flags byte plus an 8-bit value.
    if (len < 2)
    {
        return;
    }

    uint16_t bpm;
    if (data[0] & 0x01) // bit 0 set -> 16-bit value
    {
        if (len < 3)
        {
            return;
        }
        bpm = data[1] | (data[2] << 8);
    }
    else // bit 0 clear -> 8-bit value
    {
        bpm = data[1];
    }

    HeartRateSample sample{};
    sample.bpm = bpm;

    xQueueOverwrite(queue_, &sample); // never blocks
}