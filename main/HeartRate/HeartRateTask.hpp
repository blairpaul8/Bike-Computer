#pragma once

#include "../tasks/task.hpp"
#include "../drivers/ble/BLE.hpp"
#include "IHeartRate.hpp"
#include "HeartRateServer.hpp"

#include <memory>

class HeartRateTask : public Task<HeartRateTask> {
public:
    bool init();
    void run();

private:
    // Members are destroyed in reverse declaration order.
    // The HeartRate must be destroyed before its SPI bus.

    // Pointer to bluetooth driver
    std::unique_ptr<BleDriver> HeartRate_ble_;

    // Pointer to HeartRateMonitor object
    std::unique_ptr<IHeartRate> HeartRate_;

    // Pointer to HeartRateServer
    std::unique_ptr<HeartRateServer> hr_server_;
};

extern "C" void HeartRate_task_start();