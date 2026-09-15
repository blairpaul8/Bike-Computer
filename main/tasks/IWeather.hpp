#ifndef I_WEATHER_
#define I_WEATHER_

namespace weather {

class IWeather {

public:
  virtual float get_temp() = 0;
  virtual float get_humidity() = 0;
  virtual void update_sensor() = 0;

private:
  // Doxygen Comment
  virtual bool read_sensor() = 0;
};

} // end namespace weather
#endif // I_WEATHER_
