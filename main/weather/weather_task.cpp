#include "weather_task.hpp"
#include "IWeather.hpp"
#include "weather.hpp"
#include "tasks.h"

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <memory>

static const char *TAG = "WeatherTask";

bool Weather_Task::init() {
  weather_obj_ = std::make_unique<weather::Weather>();
  return weather_obj_ != nullptr;
}

void Weather_Task::run() {
  while (true) {

    // gather new sensor readings
    weather_obj_->update_sensor();

    // log readings
    ESP_LOGI(TAG, "Temp: %.1f C, Humidity: %.1f%%",
             weather_obj_->get_temp(), weather_obj_->get_humidity());

    // delay 2 seconds
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

extern "C" void weather_task_start(void) {
  static Weather_Task weatherTask;
  weatherTask.start("WeatherTask");
}
