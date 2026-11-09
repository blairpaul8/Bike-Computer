#pragma once

#include <cstdint>

class IDisplay {
public:
    virtual ~IDisplay() = default;

    virtual bool isInitialized() const = 0;
    virtual void fillDisplay(uint16_t color) = 0;
    virtual void writeChar(int x_cord, int y_cord, char x, uint16_t color = 0x0000, uint16_t bg_color = 0xFFFF) = 0;
    virtual void writeString(int x_cord, int y_cord, const char* x, uint16_t color = 0x0000, uint16_t bg_color = 0xFFFF) = 0;
};