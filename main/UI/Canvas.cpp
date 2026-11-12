#include "Canvas.hpp"

#include "font8x8_basic.h"

#include "esp_heap_caps.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>

Canvas::Canvas(int width, int maxRows)
    : width_(width), maxRows_(maxRows) {

    // allocate the memory for pixel buffer
    buffer_ = static_cast<uint16_t*>(heap_caps_malloc(
        width_ * maxRows_ * sizeof(uint16_t),
        MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
}

Canvas::~Canvas() {
    // free pixel buffer
    heap_caps_free(buffer_);
}

void Canvas::beginBand(int screenY, int rows) {
    bandY_ = screenY;
    bandRows_ = std::min(rows, maxRows_);
}

void Canvas::clear(uint16_t color) {
    // fill in the whole band
    std::fill_n(buffer_, width_ * bandRows_, convertColor(color));
}

void Canvas::fillRect(int x, int y, int w, int h, uint16_t color) {
    const int x0 = std::max(x, 0);                      // can't start left of the screen
    const int x1 = std::min(x + w, width_);             // can't extend past the right edge
    const int y0 = std::max(y, bandY_);                 // rectangle starts above this band, so start at the band's top
    const int y1 = std::min(y + h, bandY_ + bandRows_); // rectangle continues below, so stop at the band's bottom

    // x start can't be past x end
    // y start can't be past y end
    if (x0 >= x1 || y0 >= y1) return;

    const uint16_t wire = convertColor(color);
    for (int yy = y0; yy < y1; ++yy) {
        std::fill_n(&buffer_[(yy - bandY_) * width_ + x0], x1 - x0, wire);
    }
}

void Canvas::writeChar(int x_cord, int y_cord, char x, uint16_t color, uint16_t bg_color) {
    const auto index = static_cast<unsigned char>(x);
    const auto& glyph = font8x8_basic[index < 128 ? index : '?'];

    for (int row = 0; row < 8; ++row) {

        const auto bits = static_cast<uint8_t>(glyph[row]);
        for (int col = 0; col < 8; ++col) {
            const bool isSet = (bits & (1u << col)) != 0;
            setPixel(x_cord + col, y_cord + row, isSet ? color : bg_color);
        }
    }

}

void Canvas::writeString(int x_cord, int y_cord, const char* text, uint16_t color, uint16_t bg_color) {
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

uint16_t Canvas::convertColor(uint16_t color) {
    return static_cast<uint16_t>((color << 8) | (color >> 8));
}

void Canvas::setPixel(int x, int y, uint16_t color) {
    const int ly = y - bandY_;
    if (x < 0 || x >= width_ || ly < 0 || ly >= bandRows_) return;
    buffer_[ly * width_ + x] = convertColor(color);
}
