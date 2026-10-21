#ifndef GPS_TASK_HPP_
#define GPS_TASK_HPP_

#include "../tasks/task.hpp"
#include <memory>

class Gps_Task : public Task<Gps_Task> {
public:
    bool init();
    void run();
};

#endif // GPS_TASK_HPP_