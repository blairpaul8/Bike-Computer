#include "weather_task.hpp"
#include "tasks.h"

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht.h"

#define DHT_GPIO GPIO_NUM_27

static const char* TAG = "DHT11";

void Weather_Task::start()
{
    BaseType_t result = xTaskCreate(
        taskEntry,
        "WeatherTask",
        2048,
        this,
        5,
        nullptr
    );

    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create weather task");
    }
}

void Weather_Task::taskEntry(void* parameter)
{
    auto* weatherTask = static_cast<Weather_Task*>(parameter);
    weatherTask->run();

    // A FreeRTOS task must not return.
    vTaskDelete(nullptr);
}

void Weather_Task::run()
{
    float temperature = 0.0F;
    float humidity = 0.0F;

    while (true) {
        esp_err_t result = dht_read_float_data(
            DHT_TYPE_DHT11,
            DHT_GPIO,
            &humidity,
            &temperature
        );

        if (result == ESP_OK) {
            ESP_LOGI(
                TAG,
                "Temp: %.1f C, Humidity: %.1f%%",
                temperature,
                humidity
            );
        } else {
            ESP_LOGE(
                TAG,
                "Failed to read DHT11: %s",
                esp_err_to_name(result)
            );
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

extern "C" void weather_task_start(void)
{
    static Weather_Task weatherTask;
    weatherTask.start();
}