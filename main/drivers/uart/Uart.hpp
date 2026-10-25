///////////////////////////////////////////
/// @file Uart.hpp
///
/// @brief Wrapper around esp idf uart library
///        to make creating uart connections
///        easier
///
///       22 October 2026
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
  /// @brief Uart Constructor
  ///
  /// @param rx_pin GPIO pin number of the receive pin for the cooresponding Uart port
  /// @param tx_pin GPIO pin number of the transmit pin for the cooresponding Uart port
  Uart(int rx_pin = UART_PIN_NO_CHANGE, int tx_pin = UART_PIN_NO_CHANGE);

  // Destructor
  ~Uart();
  
  // Delete copy operations
  Uart(const Uart &) = delete;
  Uart &operator=(const Uart &) = delete;

  /// @brief Read the data from uart buffer
  ///
  /// @param buff    Pointer to the buffer the data should be written to
  /// @param len     length of the buffer
  /// @param timeout maximum time to block while reading the data 
  int read(uint8_t *buff, size_t len, TickType_t timeout);

  /// @brief used to check intialization state of uart
  ///
  /// @return returns true if initialized otherwise false
  bool isInitialized();

private:
  /// @brief Initialize the Uart port
  ///
  /// @return returns true if success otherwise false
  bool init();

  /// uart config struct
  uart_config_t uart_config_{};

  /// Uart port number
  uart_port_t uart_port_;

  /// GPIO pin number for Uart RX
  int rx_pin_;

  /// GPIO pin number for Uart TX
  int tx_pin_;

  /// Buffer size of the Uart
  int uart_buffer_;

  /// initialization flag
  bool initialized_;

  /// Uart tag used for the ESP logger
  static constexpr const char *UART_TAG = "Uart";
};
} // namespace uart

#endif // !UART_HPP_
