// Renderer.hpp
#pragma once

#include "Canvas.hpp"
#include "Widgets/Widget.hpp"
#include "../display/IDisplay.hpp"

class Renderer {
public:
    explicit Renderer(IDisplay& display);

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void setRoot(Widget* root) { root_ = root; }   // the active screen
    void setBackground(uint16_t color) { background_ = color; }

    void renderFrame();

private:
    static constexpr int TRANSFER_ROWS = 20;

    IDisplay& display_;
    Canvas    canvas_;
    Widget*   root_ = nullptr;
    uint16_t  background_ = 0x0000;
};