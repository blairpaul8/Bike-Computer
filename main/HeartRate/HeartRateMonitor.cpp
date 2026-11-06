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
    // TODO: move the parsing from the old NOTIFY_RX case here.
    //   data[0] is the flags byte.
    //   bit 0 set   -> 16-bit value: data[1] | (data[2] << 8)
    //   bit 0 clear -> 8-bit value:  data[1]
    // Check len first so you never read past the end
    // (need len >= 2, or len >= 3 for the 16-bit form).

    HeartRateSample sample{};
    // sample.bpm = ...;

    xQueueOverwrite(queue_, &sample); // never blocks
}