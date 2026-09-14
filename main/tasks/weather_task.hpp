#ifndef WEATHER_TASK_HPP
#define WEATHER_TASK_HPP

class Weather_Task {
public:
    void start();

private:
    static void taskEntry(void* parameter);
    void run();
};

#endif