#ifndef WEATHER_HPP_
#define WEATHER_HPP_

#include "IWeather.hpp"

namespace weather {

class Weather : public weather::IWeather {
public:
  // default constructor
  Weather();
  float get_temp() override;
  float get_humidity() override;

private:
  float temperature_;
  float humidity_;

  bool read_sensor(float &temp, float &humidity) override;
};

} // namespace weather
//
#endif // WEATHER_HPP_
