// HeartRateMonitor.hpp
#pragma once
#include "IHeartRate.hpp"
#include "drivers/ble/BLE.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class HeartRateMonitor : public IHeartRate
{
public:
    explicit HeartRateMonitor(BleDriver &ble) : ble_(ble) {}

    // initialize the HeartRateMonitor class
    // inits queue.
    bool init() override;

    // pops HR reading off the queue and writes to a HeartRateSample
    bool read(HeartRateSample &out, uint32_t timeoutMs) override;

    // Used to check if connected to a HR monitor.
    bool isConnected() const override { return ble_.isConnected(); }

private:
    // Heart Rate service and Heart Rate Measurement characteristic.
    static constexpr uint16_t kServiceUuid = 0x180D;

    // GRM Pro Specific Heart Monitor Id.
    static constexpr uint16_t kCharUuid = 0x2A37;

    // BleDriver calls this on the NimBLE task. It forwards to onNotify().
    // Thunk is an adpater function that bridges a C-style callback and C++
    // a C++ member function.
    static void notifyThunk(const uint8_t *data, size_t len, void *ctx);

    void onNotify(const uint8_t *data, size_t len);

    // Reference to Bluetooth driver
    BleDriver &ble_;

    // Queue that BleDriver pushs HR readings onto, and the read method
    // of this class pops them off of.
    QueueHandle_t queue_ = nullptr; // holds the latest sample (length 1, overwritten)
};