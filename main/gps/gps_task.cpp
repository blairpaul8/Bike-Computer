
#include "gps_task.hpp"

#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <memory>
#include <cstdint>

static const char* TAG = "GpsTask";

static constexpr uart_port_t GPS_UART = UART_NUM_2;
static constexpr int GPS_RX_PIN = 16;
static constexpr int UART_BUFFER_SIZE = 2048;

bool Gps_Task::init() {
    return true;
}

void Gps_Task::run() {
uart_config_t uart_config = {};

uart_config.baud_rate = 9600;
uart_config.data_bits = UART_DATA_8_BITS;
uart_config.parity = UART_PARITY_DISABLE;
uart_config.stop_bits = UART_STOP_BITS_1;
uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
uart_config.rx_flow_ctrl_thresh = 0;

    ESP_ERROR_CHECK(uart_param_config(GPS_UART, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(
        GPS_UART,
        UART_PIN_NO_CHANGE,
        GPS_RX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    ));

    ESP_ERROR_CHECK(uart_driver_install(
        GPS_UART,
        UART_BUFFER_SIZE,
        0,
        0,
        nullptr,
        0
    ));

uint8_t data[128];

while (true) {
    int length = uart_read_bytes(
        GPS_UART,
        data,
        sizeof(data) - 1,
        pdMS_TO_TICKS(1000)
    );

    ESP_LOGI(TAG, "UART received %d bytes", length);

if (length > 0) {
    ESP_LOGI(TAG, "Received %d bytes:", length);
    ESP_LOG_BUFFER_HEX(TAG, data, length);
}
}

    uart_driver_delete(GPS_UART);
}

extern "C" void gps_task_start(void*) {
    static Gps_Task gpsTask;
    gpsTask.start("GpsTask");
}
