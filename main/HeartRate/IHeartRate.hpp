// i_heart_rate.hpp
#pragma once
#include <cstdint>

struct HeartRateSample
{
    uint16_t bpm;
};

// Same idea as IGps: the task only knows about this interface,
// not about BLE or the queue underneath.
class IHeartRate
{
public:
    virtual ~IHeartRate() = default;

    // Start the sensor (creates the queue and starts BLE).
    virtual bool init() = 0;

    // Wait up to timeoutMs for a new sample.
    // Returns true if `out` was filled, false on timeout.
    virtual bool read(HeartRateSample &out, uint32_t timeoutMs) = 0;

    // True while the heart rate strap is connected.
    virtual bool isConnected() const = 0;
};