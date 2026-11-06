// ble_driver.hpp
#pragma once
#include <cstdint>
#include <cstddef>

// Called on the NimBLE host task for every notification.
// Keep it short and never block.
using BleNotifyCallback = void (*)(const uint8_t *data, size_t len, void *ctx);

// Generic BLE central driver (same role as Uart for GPS).
// Knows nothing about heart rate: give it a service UUID and a characteristic UUID,
// and it finds a matching device, connects, subscribes, and forwards raw
// notification bytes to your callback.
class BleDriver
{
public:
    // Start BLE, find a device advertising serviceUuid, connect,
    // and subscribe to charUuid. Returns false if setup failed.
    bool begin(uint16_t serviceUuid, uint16_t charUuid, BleNotifyCallback cb, void *ctx);

    // True while a device is connected.
    bool isConnected() const;
};