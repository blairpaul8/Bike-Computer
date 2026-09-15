#ifndef WEATHER_TASK_HPP_
#define WEATHER_TASK_HPP_

#include "../tasks/task.hpp"
#include "weather.hpp"
#include <memory>

class Weather_Task : public Task<Weather_Task> {
public:
    bool init();
    void run();

private:
    // pointer to the weather object
    std::unique_ptr<weather::IWeather> weather_obj_;
};

#endif // WEATHER_TASK_HPP_