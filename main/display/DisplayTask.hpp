#pragma once

#include <memory>

#include "../tasks/task.hpp"
#include "../drivers/spi/Spi.hpp"
#include "IDisplay.hpp"

class DisplayTask : public Task<DisplayTask> {
public:
    bool init();
    void run();

private:
    // Members are destroyed in reverse declaration order.
    // The display must be destroyed before its SPI bus.
    std::unique_ptr<spi::Spi> display_spi_;
    std::unique_ptr<IDisplay> display_;
};

extern "C" void display_task_start();