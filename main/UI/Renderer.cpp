// Renderer.cpp
#include "Renderer.hpp"

#include <algorithm>

Renderer::Renderer(IDisplay& display)
    : display_(display),
      canvas_(display.width(), TRANSFER_ROWS) {}

void Renderer::renderFrame() {
    if (!root_ || !display_.isInitialized()) return;

    const int height = display_.height();
    const int width  = display_.width();

    for (int y = 0; y < height; y += TRANSFER_ROWS) {
        const int rows = std::min(TRANSFER_ROWS, height - y);

        canvas_.beginBand(y, rows);
        canvas_.clear(background_);
        root_->draw(canvas_);

        display_.flush(0, y, width, y + rows, canvas_.getBuffer());
        display_.waitDone();
    }
}