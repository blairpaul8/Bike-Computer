// RideData.hpp
#pragma once

#include <stdint.h>

typedef struct 
{
   uint32_t distance;       ///< Total distance traveled so far.
   uint32_t elevation;      ///< Total elevation gained so far.
   uint32_t speed;          ///< Current speed.
   uint32_t avg_speed;      ///< Average speed so far.
   uint32_t heart_rate;     ///< Current Heart rate reading..
   uint32_t avg_heart_rate; ///< Average heart rate so far.

} RideData;
