#include "weather.hpp"
#include "dht.h"
#include "esp_err.h"
#include "esp_log.h"

#define DHT_GPIO GPIO_NUM_27
static const char *TAG = "DHT11";

using namespace weather;

Weather::Weather() : temperature_(0.0), humidity_(0.0) {}

float Weather::get_temp() { return temperature_; }

float Weather::get_humidity() { return humidity_; }

// silly method to keep read_sensor private.
// just as an example
void Weather::update_sensor() {
  this->read_sensor();
}

bool Weather::read_sensor() {
  esp_err_t res = dht_read_float_data(DHT_TYPE_DHT11, DHT_GPIO, &this->temperature_, &this->humidity_);

  if (res != ESP_OK) {
    ESP_LOGI(TAG, "Failed to read DHT11: %s", esp_err_to_name(res));
    return false;
  }

  return true;
}
