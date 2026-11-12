// ui/widgets/HelloWidget.hpp
#pragma once

#include "Widget.hpp"
#include "../Canvas.hpp"

// Minimal example widget: draws a fixed two-line message.
class HelloWidget : public Widget {
public:
    HelloWidget() : Widget({72, 100, 200, 16}) {}

    void draw(Canvas& c) override {
        c.writeString(bounds_.x, bounds_.y,
                      "Well hello there ELI!\nHow is your day going?");


        c.writeString(bounds_.x, bounds_.y + 200,
                      "Test Second string\nShould be about 200 pixels\nbelow the greeting");
    }
};