#pragma once

#include "driver/spi_master.h"

namespace spi {

class Spi {
public:
    /// @param max_transfer Maximum transfer size in bytes
    explicit Spi(int max_transfer);

    ~Spi();

    Spi(const Spi&) = delete;
    Spi& operator=(const Spi&) = delete;

    bool isInitialized() const;
    spi_host_device_t getHost() const;

private:
    bool init();

    spi_bus_config_t bus_config_{};

    int max_transfer_;
    bool initialized_;

    static constexpr const char* SPI_TAG = "Spi";
};

} // namespace spi