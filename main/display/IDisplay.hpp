#pragma once

#include <cstdint>

class IDisplay {
public:
    virtual ~IDisplay() = default;

    virtual bool isInitialized() const = 0;
    virtual void fillDisplay(uint16_t color) = 0;
};