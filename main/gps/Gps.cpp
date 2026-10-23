/////////////////////////////////////////////////
/// @file Gps.cpp
///
/// @brief Main Gps class that handles retrieving,
///        parsing, and verifying gps data.
///    
///
///  22 October 2026
/////////////////////////////////////////////////
#include "Gps.hpp"
#include "GpsFix.hpp"
#include <cstdint>

using namespace gps;

  Gps::Gps() {}

  bool Gps::update(uint32_t timeout_ms) {}

  GpsFix Gps::getFix() const {}
  
  bool Gps::hasFix() const {}

  bool parseGpsSentence() {}

  bool Gps::assembleGpsSentence() {}

  // Add whatever functions or methods are needed.
  // if its just a helper function put it at the bottom of this file
  // if it should be a method of the class add it to the .hpp file
