// ui/widgets/HelloWidget.hpp
#pragma once

#include "Widget.hpp"
#include "../Canvas.hpp"

// Minimal example widget: draws a fixed two-line message.
class HelloWidget : public Widget {
public:
    HelloWidget() : Widget({kX, kY, kWidth, kHeight}) {}

    void draw(Canvas& c) override;

private:
    // Widget bounds (screen coordinates)
    static constexpr int kX      = 72;
    static constexpr int kY      = 100;
    static constexpr int kWidth  = 200;
    static constexpr int kHeight = 16;

    // Placement of fields inside the widget
    static constexpr int kGreetingStringX = kX + 0;
    static constexpr int kGreetingStringY = kY + 0;
    static constexpr int kSecondStringX   = kX + 0;
    static constexpr int kSecondStringY   = kY + 200;

    // Add other member variables here
    // ex.
    // uint8_t hr_;
};