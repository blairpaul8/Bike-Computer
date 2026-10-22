#include "Uart.hpp"

#include "esp_err.h"
#include "esp_log.h"
#include <cstdint>

using namespace uart;

Uart::Uart(int rx_pin, int tx_pin) : 
  uart_port_(UART_NUM_2),
  rx_pin_(rx_pin),
  tx_pin_(tx_pin),
  uart_buffer_(2048),
  initialized_(false)
{

  initialized_ = init();
  // If init fails log failure
  if (!initialized_) {
    ESP_LOGE(UART_TAG, "Failed to initialize");
  }
}

Uart::~Uart() {
  // remove the uart driver
  if (initialized_) {
    uart_driver_delete(uart_port_);
  }
}

bool Uart::init() {

  // Setup config struct
  uart_config_.baud_rate = 9600;
  uart_config_.data_bits = UART_DATA_8_BITS;
  uart_config_.parity = UART_PARITY_DISABLE;
  uart_config_.stop_bits = UART_STOP_BITS_1;
  uart_config_.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
  uart_config_.rx_flow_ctrl_thresh = 0;

  esp_err_t config_err = uart_param_config(uart_port_, &uart_config_);
  if (config_err != ESP_OK) {
    ESP_LOGE(UART_TAG, "param config failed: 0x%x", config_err);
    return false;
  }

  esp_err_t set_pin_err = uart_set_pin(uart_port_, UART_PIN_NO_CHANGE, rx_pin_,
                                       UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  if (set_pin_err != ESP_OK) {
    ESP_LOGE(UART_TAG, "set pin failed: 0x%x", set_pin_err);
    return false;
  }

  esp_err_t driver_install_err = uart_driver_install(uart_port_, uart_buffer_, 0, 0, nullptr, 0);
  if (driver_install_err != ESP_OK) {
    ESP_LOGE(UART_TAG, "driver install failed: 0x%x", driver_install_err);
    return false;
  }

  return true;
}

int Uart::read(uint8_t *buff, size_t len, TickType_t timeout) {
  // read the peripheral device
  return uart_read_bytes(uart_port_, buff, len, timeout);
}

bool Uart::isInitialized() {
  // return the state
  return initialized_;
}
