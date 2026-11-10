// Widget.hpp
#pragma once

#include "../Canvas.hpp"

struct Rect {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    // true if this rect overlaps the horizontal strip [bandY, bandY + bandRows)
    bool overlapsBand(int bandY, int bandRows) const {
        return y < bandY + bandRows && y + h > bandY;
    }
};

class Widget {
public:
    explicit Widget(Rect bounds = {}) : bounds_(bounds) {}
    virtual ~Widget() = default;

    virtual void draw(Canvas& canvas) = 0;

    const Rect& bounds() const { return bounds_; }
    void setBounds(Rect r) { bounds_ = r; }

protected:
    Rect bounds_;
};