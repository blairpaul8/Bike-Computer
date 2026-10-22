///////////////////////////////////////////
// Uart.hpp
//
// Description:
//  Wrapper around esp idf uart library
//  to make creating uart connections
//  easier
//
//  22 October 2026
///////////////////////////////////////////
#ifndef UART_HPP_
#define UART_HPP_

#include "driver/uart.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

#include "cstdint"
#include "cstddef"

namespace uart {

class Uart {
public:
  // Constructor
  Uart(int rx_pin = UART_PIN_NO_CHANGE, int tx_pin = UART_PIN_NO_CHANGE);

  // Destructor
  ~Uart();
  
  // Delete copy operations
  Uart(const Uart &) = delete;
  Uart &operator=(const Uart &) = delete;

  int read(uint8_t *buff, size_t len, TickType_t timeout);

  // Public method to check intialization state of uart
  bool isInitialized();

private:
  // initialize uart
  bool init();

  // uart config struct
  uart_config_t uart_config_{};

  // Uart port number
  uart_port_t uart_port_;

  // GPIO pin number for Uart RX
  int rx_pin_;

  // GPIO pin number for Uart TX
  int tx_pin_;

  // Buffer size of the Uart
  int uart_buffer_;

  // initialization flag
  bool initialized_;

  static constexpr const char *UART_TAG = "Uart";
};
} // namespace uart

#endif // !UART_HPP_
