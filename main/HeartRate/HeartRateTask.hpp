#pragma once

#include <memory>
#include "../tasks/task.hpp"

#include "../drivers/ble/BLE.hpp"
#include "IHeartRate.hpp"

class HeartRateTask : public Task<HeartRateTask> {
public:
    bool init();
    void run();

private:
    // Members are destroyed in reverse declaration order.
    // The HeartRate must be destroyed before its SPI bus.
    std::unique_ptr<BleDriver> HeartRate_ble_;
    std::unique_ptr<IHeartRate> HeartRate_;
};

extern "C" void HeartRate_task_start();