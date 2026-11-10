#pragma once

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include <cstdint>

class Canvas {
public:
    // constructor
    explicit Canvas(int width, int maxRows);

    // Destructor
    ~Canvas();

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    void beginBand(int screenY, int rows);

    void clear(uint16_t color);

    void fillRect(int x, int y, int w, int h, uint16_t color);

    void writeChar(int x_cord, int y_cord, char x, uint16_t color = 0xFFFF, uint16_t bg_color = 0x0000);

    void writeString(int x_cord, int y_cord, const char* x, uint16_t color = 0xFFFF, uint16_t bg_color = 0x0000);

    const uint16_t* getBuffer() const { return buffer_; }

private:
    uint16_t convertColor(uint16_t color);

    void setPixel(int x, int y, uint16_t color);

    uint16_t* buffer_ = nullptr;
    int width_;
    int maxRows_;
    int bandY_ = 0;
    int bandRows_ = 0;


    
};
