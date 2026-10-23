///////////////////////////////////////////
// GpsFix.hpp
//
// Description:
//  Defines a valid GPS reading with all the
//  needed GPS data in one struct
//
//  22 October 2026
///////////////////////////////////////////
#ifndef GPS_FIX_HPP_
#define GPS_FIX_HPP_

#include <cstdint>

namespace gps {

struct GpsFix {
  bool valid{false};
  double lat_deg{0};
  double lon_deg{0};
  float speed_mps{0};
  float altitude_m{0};
  uint8_t satellites{0}; // Might delete this
  // add date/time fields
};

} // namespace gps
#endif