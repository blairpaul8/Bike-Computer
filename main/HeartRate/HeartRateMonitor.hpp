// HeartRateMonitor.hpp
#pragma once
#include "IHeartRate.hpp"
#include "drivers/ble/BLE.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Same role as the Gps class: holds the sensor-specific logic and uses the
// driver underneath (BleDriver here, Uart for Gps).
class HeartRateMonitor : public IHeartRate
{
public:
    explicit HeartRateMonitor(BleDriver &ble) : ble_(ble) {}

    bool init() override;
    bool read(HeartRateSample &out, uint32_t timeoutMs) override;
    bool isConnected() const override { return ble_.isConnected(); }

private:
    // Heart Rate service and Heart Rate Measurement characteristic.
    static constexpr uint16_t kServiceUuid = 0x180D;
    static constexpr uint16_t kCharUuid = 0x2A37;

    // BleDriver calls this on the NimBLE task. It forwards to onNotify().
    static void notifyThunk(const uint8_t *data, size_t len, void *ctx);
    void onNotify(const uint8_t *data, size_t len);

    BleDriver &ble_;
    QueueHandle_t queue_ = nullptr; // holds the latest sample (length 1, overwritten)
};