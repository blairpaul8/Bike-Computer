#include "Spi.hpp"

#include "esp_err.h"
#include "esp_log.h"

namespace spi {

Spi::Spi(int max_transfer)
    : max_transfer_(max_transfer),
      initialized_(false)
{
    initialized_ = init();
}

Spi::~Spi() {
    if (initialized_) {
        esp_err_t err = spi_bus_free(getHost());

        if (err != ESP_OK) {
            ESP_LOGE(
                SPI_TAG,
                "Failed to free SPI bus: %s",
                esp_err_to_name(err)
            );
        }
    }
}

bool Spi::init() {
    if (max_transfer_ <= 0) {
        ESP_LOGE(SPI_TAG, "Maximum transfer size must be positive");
        return false;
    }

    bus_config_.sclk_io_num = 18;
    bus_config_.mosi_io_num = 23;
    bus_config_.miso_io_num = 19;
    bus_config_.quadwp_io_num = -1;
    bus_config_.quadhd_io_num = -1;
    bus_config_.max_transfer_sz = max_transfer_;

    esp_err_t err = spi_bus_initialize(
        getHost(),
        &bus_config_,
        SPI_DMA_CH_AUTO
    );

    if (err != ESP_OK) {
        ESP_LOGE(
            SPI_TAG,
            "Failed to initialize SPI bus: %s",
            esp_err_to_name(err)
        );

        return false;
    }

    return true;
}

bool Spi::isInitialized() const {
    return initialized_;
}

spi_host_device_t Spi::getHost() const {
    return SPI2_HOST;
}

} // namespace spi