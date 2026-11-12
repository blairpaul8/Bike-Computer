#pragma once
#include <cstdint>

class IDisplay {
public:
    virtual ~IDisplay() = default;

    virtual bool isInitialized() const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    // Send a rectangle of RGB565 pixels (already in wire byte order).
    // x1/y1 are exclusive, matching esp_lcd_panel_draw_bitmap.
    virtual void flush(int x0, int y0, int x1, int y1, const uint16_t* pixels) = 0;

    // Block until the last flush has finished reading its buffer.
    virtual void waitDone() = 0;
};