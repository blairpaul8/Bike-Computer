/////////////////////////////////////////////////
/// @file IGps.hpp
///
/// @brief Gps interface that defines the main 
///        Gps Api
///
///  22 October 2026
/////////////////////////////////////////////////
#ifndef IGPS_HPP_
#define IGPS_HPP_

#include "GpsFix.hpp"
#include <cstdint>

namespace gps {

class IGps {

public:
  /// @brief default destructor
  virtual ~IGps() = default;

  /// @brief On call to update an updated
  /// reading from the GPS module will be retrieved
  ///
  /// @param timeout_ms maximum time to block while waiting for updated Gps fix
  /// @return true on success otherwise false
  virtual bool update(uint32_t timeout_ms) = 0;

  /// @brief getFix should only be called after hasFix has been
  /// called and returns true otherwise the result may be
  /// stale or bad GPS data.
  ///
  /// @return a GpsFix struct with valid Gps data.
  virtual GpsFix getFix() const = 0;
  
  /// @brief Used to verify that a valid GpsFix is ready.
  ///
  /// @return true when a valid GpsFix struct is filled
  virtual bool hasFix() const = 0;
};

} // end namespace gps
#endif // IGPS_HPP_
