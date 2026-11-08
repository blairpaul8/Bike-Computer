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
#include "esp_log.h"


using namespace gps;

static const char* TAG = "Gps";

  Gps::Gps() {}

  bool Gps::update(uint32_t timeout_ms) {}

  GpsFix Gps::getFix() const {}
  
  bool Gps::hasFix() const {}

  bool parseGpsSentence() {}

  bool Gps::assembleGpsSentence(char c) {
    if (c == '$') {
      line_len_ = 0;
      in_sentence_ = true;
    }

    if (!in_sentence_ || c == '\r') {
      return false;
    }

    if (c == '\n') {
      line_[line_len_] = '\0';
      in_sentence_ = false;
      return true;
    }

    if (line_len_ < sizeof(line_) - 1) {
      line_[line_len_++] = c;
    } else {
      in_sentence_ = false;
    }

    return false;
  }

  void Gps::feed(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
      if (assembleGpsSentence(static_cast<char>(data[i]))) {
        ESP_LOGI(TAG, "Sentence: %s", line_);
      }
    }
  }

  // Add whatever functions or methods are needed.
  // if its just a helper function put it at the bottom of this file
  // if it should be a method of the class add it to the .hpp file
