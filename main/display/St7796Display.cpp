#include "St7796Display.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_st7796.h"

#include "font8x8_basic.h"

St7796Display::St7796Display(spi::Spi& spiBus) {
    if (!spiBus.isInitialized()) {
        return;
    }

    esp_lcd_panel_io_spi_config_t io_config{};

    io_config.cs_gpio_num = GPIO_NUM_5;
    io_config.dc_gpio_num = GPIO_NUM_21;
    io_config.spi_mode = 0;
    io_config.pclk_hz = 10 * 1000 * 1000;
    io_config.trans_queue_depth = 10;
    io_config.lcd_cmd_bits = 8;
    io_config.lcd_param_bits = 8;

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
        spiBus.getHost(), &io_config, &io_
    ));

    esp_lcd_panel_dev_config_t panel_config{};

    panel_config.reset_gpio_num = GPIO_NUM_17;
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;
    panel_config.bits_per_pixel = 16;

    ESP_ERROR_CHECK(esp_lcd_new_panel_st7796(
        io_, &panel_config, &panel_
    ));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_, true, false));

    buffer_ = static_cast<uint16_t*>(
        heap_caps_malloc(
            PIXELS * sizeof(uint16_t),
            MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL
        )
    );

    ESP_ERROR_CHECK(buffer_ ? ESP_OK : ESP_ERR_NO_MEM);

    initialized_ = true;
}

St7796Display::~St7796Display() {
    heap_caps_free(buffer_);

    if (io_) {
        // Finish pending transfers before deleting LCD resources.
        ESP_ERROR_CHECK(
            esp_lcd_panel_io_tx_param(io_, -1, nullptr, 0)
        );
    }

    if (panel_) {
        ESP_ERROR_CHECK(esp_lcd_panel_del(panel_));
    }

    if (io_) {
        ESP_ERROR_CHECK(esp_lcd_panel_io_del(io_));
    }
}

bool St7796Display::isInitialized() const {
    return initialized_;
}

void St7796Display::fillDisplay(uint16_t color) {
    if (!initialized_) {
        return;
    }


    ESP_ERROR_CHECK(buffer_ ? ESP_OK : ESP_ERR_NO_MEM);

    // Swap bytes for the LCD's RGB565 transfer order.
    uint16_t wireColor = static_cast<uint16_t>(
        (color << 8) | (color >> 8)
    );

    std::fill_n(buffer_, PIXELS, wireColor);

    for (int y = 0; y < HEIGHT; y += TRANSFER_ROWS) {
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(
            panel_,
            0,
            y,
            WIDTH,
            std::min(y + TRANSFER_ROWS, HEIGHT),
            buffer_
        ));
    }

    // Wait for SPI to finish reading the buffer.
    ESP_ERROR_CHECK(
        esp_lcd_panel_io_tx_param(io_, -1, nullptr, 0)
    );

    heap_caps_free(buffer_);
}

uint16_t St7796Display::convertColor(uint16_t color) {
    return static_cast<uint16_t>((color << 8) | (color >> 8));
}

void St7796Display::addLetterToBuffer(char x, uint16_t color, uint16_t bg_color) {
    const auto index = static_cast<unsigned char>(x);
    const auto& glyph = font8x8_basic[x < 128 ? index : '?'];

    for (int row = 0; row < 8; ++row) {

        const auto bits = static_cast<uint8_t>(glyph[row]);
        for (int col = 0; col < 8; ++col) {
            const bool isSet = (bits & (1u << col)) != 0;
            buffer_[row * 8 + col] = isSet ? color : bg_color;
        }
    }
}

void St7796Display::writeChar(int x_cord, int y_cord, char x, uint16_t color, uint16_t bg_color) {
    if(!initialized_) {
        return;
    }

    ESP_ERROR_CHECK(buffer_ ? ESP_OK : ESP_ERR_NO_MEM);

    const uint16_t c_color = convertColor(color);
    const uint16_t b_color = convertColor(bg_color);

    addLetterToBuffer(x, c_color, b_color);

    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(
        panel_,
        x_cord,
        y_cord,
        x_cord + 8,
        y_cord + 8,
        buffer_
    ));
    // Wait for SPI to finish reading the buffer.
    ESP_ERROR_CHECK(
        esp_lcd_panel_io_tx_param(io_, -1, nullptr, 0)
    );

}

void St7796Display::writeString(int x_cord, int y_cord, const char* text, uint16_t color, uint16_t bg_color) {
    if(!initialized_) {
        return;
    }

    int start_x = x_cord;

    while(*text != '\0') {
        if(*text == '\n') {
            x_cord = start_x;
            y_cord += 8;
        } else {
            writeChar(x_cord, y_cord, *text, color, bg_color);
            x_cord += 8;
        }
        ++text;
    };
}