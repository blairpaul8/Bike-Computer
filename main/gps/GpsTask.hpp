///////////////////////////////////////////
// GpsTask.hpp
//
// Description:
//  Main Gps task that handles initializing
//  the neccessary objects. It also maintains
//  main Gps logic loop.
//
//  22 October 2026
///////////////////////////////////////////

#ifndef GPS_TASK_HPP_
#define GPS_TASK_HPP_

#include "../tasks/task.hpp"
#include "../drivers/uart/Uart.hpp"
#include <memory>

class GpsTask : public Task<GpsTask> {
public:
    bool init();
    void run();

private:
    // Pointer to the Uart obj
    std::unique_ptr<uart::Uart> gps_uart_;

    // GPIO pin for the Gps Uart connection
    static constexpr int gps_uart_pin_ = 16;

    // Tag for the logger
    static constexpr const char *TAG = "GpsTask";
};

#endif // GPS_TASK_HPP_