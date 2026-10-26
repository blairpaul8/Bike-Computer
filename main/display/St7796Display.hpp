#pragma once

#include "IDisplay.hpp"
#include "../drivers/spi/Spi.hpp"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include <cstdint>

class St7796Display : public IDisplay {
public:
    explicit St7796Display(spi::Spi& spiBus);
    ~St7796Display() override;

    St7796Display(const St7796Display&) = delete;
    St7796Display& operator=(const St7796Display&) = delete;

    bool isInitialized() const override;
    void fillDisplay(uint16_t color) override;

    static constexpr int WIDTH = 320;
    static constexpr int HEIGHT = 480;
    static constexpr int TRANSFER_ROWS = 20;
    static constexpr size_t PIXELS = 6400;

private:
    esp_lcd_panel_handle_t panel_ = nullptr;
    esp_lcd_panel_io_handle_t io_ = nullptr;

    uint16_t* buffer_ = nullptr;

    bool initialized_ = false;
};