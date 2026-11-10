#pragma once

#include "IDisplay.hpp"
#include "../drivers/spi/Spi.hpp"

#include "../UI/Canvas.hpp"
#include "../UI/Renderer.hpp"

#include "../tasks/task.hpp"
#include <memory>

class DisplayTask : public Task<DisplayTask> {
public:
    bool init();
    void run();

private:
    // Members are destroyed in reverse declaration order.
    // The display must be destroyed before its SPI bus.
    std::unique_ptr<spi::Spi> display_spi_;

    std::unique_ptr<IDisplay> display_;

    std::unique_ptr<Renderer> renderer_;
};

extern "C" void display_task_start();