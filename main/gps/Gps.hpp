/////////////////////////////////////////////////
/// @file Gps.hpp
///
/// @brief Main Gps class that handles retrieving,
///        parsing, and verifying gps data.
///    
///
///  22 October 2026
/////////////////////////////////////////////////
#ifndef GPS_HPP_
#define GPS_HPP_

#include "IGps.hpp"
#include "GpsFix.hpp"
#include <cstdint>

namespace gps {

class Gps : public IGps {

public:
  /// @brief Constructor
  Gps();

  /// @brief default destructor
  ~Gps() = default;

  /// @brief On call to update an updated
  /// reading from the GPS module will be retrieved
  ///
  /// @param timeout_ms maximum time to block while waiting for updated Gps fix
  /// @return true on success otherwise false
  bool update(uint32_t timeout_ms);

  /// @brief getFix should only be called after hasFix has been
  /// called and returns true otherwise the result may be
  /// stale or bad GPS data.
  ///
  /// @return a GpsFix struct with valid Gps data.
  GpsFix getFix() const;
  
  /// @brief Used to verify that a valid GpsFix is ready.
  ///
  /// @return true when a valid GpsFix struct is filled
  bool hasFix() const;
private:

  /// @brief Parses the data fields from a Nmea gps sentence
  ///
  /// @return 
  bool parseGpsSentence();

  /// @brief When reading Gps data from the uart buffer
  /// it is unknown where in the Gps sentence it will start.
  /// Thus this assembler looks byte by byte for '\n' to know
  /// when a sentence begins and ends.
  ///
  /// @return true if a rmc or gga sentence is found
  bool assembleGpsSentence();

  /// @brief stores the current $GPRMC Sentence
  char rmc_[128]{};

  /// @brief stores the current $GPGGA Sentence
  char gga_[128]{};
};

} // end namespace gps
#endif // GPS_HPP_
