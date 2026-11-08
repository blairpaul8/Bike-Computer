///////////////////////////////////////////
// GpsTask.cpp
//
// Description:
//  Main Gps task that handles initializing
//  the neccessary objects. It also maintains
//  main Gps logic loop.
//
//  22 October 2026
///////////////////////////////////////////
#include "GpsTask.hpp"
#include "../drivers/uart/Uart.hpp"

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cstdint>
#include <memory>

bool GpsTask::init() { 
  // Create uart obj
  gps_uart_ = std::make_unique<uart::Uart>(gps_uart_pin_);
  if (!gps_uart_->isInitialized()) {
    ESP_LOGE(TAG, "failed to create gps_uart obj");
    return false;
  }

  // Create gps obj
  gps_ = std::make_unique<gps::Gps>();
  if (!gps_->isInitialized()) {
    ESP_LOGE(TAG, "failed to create gps_ obj");
    return false;
  }


  return true; 
}

void GpsTask::run() {
  // Note: the Gps string may come in pieces
  // we can try making this bigger but due to the 
  // way uart works it may not transmit fast enough
  // so we might have to accumulate multiple transmission
  // and look for the '\n' to find the end of the string.
  uint8_t data[128];

  while (true) {
    int length = gps_uart_->read(data, sizeof(data) - 1, pdMS_TO_TICKS(1000));

    if (length > 0) {
      data[length] = '\0';
      gps_->feed(data, length);
    }
  }
}

extern "C" void gps_task_start(void *) {
  static GpsTask gpsTask;
  gpsTask.start("GpsTask");
}
