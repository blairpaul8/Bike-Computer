#include "DisplayTask.hpp"
#include "St7796Display.hpp"

#include <cstdint>
#include <memory>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

bool DisplayTask::init() {
    // Avoid initializing the same bus twice.
    if (display_) {
        return display_->isInitialized();
    }

    constexpr int TRANSFER_BYTES =
        St7796Display::WIDTH *
        St7796Display::TRANSFER_ROWS *
        sizeof(uint16_t);

    display_spi_ = std::make_unique<spi::Spi>(TRANSFER_BYTES);

    if (!display_spi_->isInitialized()) {
        display_spi_.reset();
        return false;
    }

    display_ = std::make_unique<St7796Display>(*display_spi_);

    if (!display_->isInitialized()) {
        display_.reset();
        display_spi_.reset();
        return false;
    }

    return true;
}

void DisplayTask::run() {
    display_->fillDisplay(0xF800);

    while (true) {
        // Add screen updates here later.
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

extern "C" void display_task_start() {
    static DisplayTask displayTask;
    displayTask.start("DisplayTask");
}