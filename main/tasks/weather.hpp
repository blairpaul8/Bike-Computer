#ifndef WEATHER_HPP_
#define WEATHER_HPP_

#include "IWeather.hpp"

namespace weather {

class Weather : public weather::IWeather {
public:
  // default constructor
  Weather();

  // retrieve temperature reading
  float get_temp() override;

  // retrieve humidity reading
  float get_humidity() override;

  // refresh the temp and humidity values
  void update_sensor() override;

private:
  float temperature_;
  float humidity_;

  bool read_sensor() override;
};

} // namespace weather
//
#endif // WEATHER_HPP_
