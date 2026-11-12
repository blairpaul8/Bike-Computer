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
#include <cstring>
#include "esp_log.h"


using namespace gps;

static const char* TAG = "Gps";

  Gps::Gps() {}

  bool Gps::update(uint32_t timeout_ms) { (void)timeout_ms; return false; }

  GpsFix Gps::getFix() const { return GpsFix(); }
  
  bool Gps::hasFix() const { return false; }

  bool Gps::parseGpsSentence() { return false; }

  bool Gps::assembleGpsSentence(char c) {

    // $ is the start of a sentence. Ex. $GPGSV,3,2,12,12,07,036,26,13,14,041,21,23,10,144,,25,36,070,24*7D
    if (c == '$') {
      line_len_ = 0;
      in_sentence_ = true;
    }

    // sentences end with \r\n so we want to ignore the \r
    if (!in_sentence_ || c == '\r') {
      return false;
    }

    // reached the end of the sentence which means that line_ has the full sentence
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

  void Gps::storeSentence() {
    
    // Sanity check to make sure we won't look past memory
    if (std::strlen(line_) < 6) {
      return;
    }

    // Checks if the sentence is RMC or GGA. Format is $GPGGA so when we check we have to look 3 bytes ahead
    if (std::strncmp(line_ + 3, "RMC", 3) == 0) {
      std::strncpy(rmc_, line_, sizeof(rmc_) - 1);
      rmc_[sizeof(rmc_) - 1] = '\0';
      ESP_LOGI(TAG, "RMC stored: %s", rmc_);
    } else if (std::strncmp(line_ + 3, "GGA", 3) == 0) {
      std::strncpy(gga_, line_, sizeof(gga_) - 1);
      gga_[sizeof(gga_) - 1] = '\0';
      ESP_LOGI(TAG, "GGA stored: %s", gga_);
    }
  }

  void Gps::feed(const uint8_t* data, size_t len) {
    // Passes one byte at a time to the assembler
    for (size_t i = 0; i < len; i++) {
      if (assembleGpsSentence(static_cast<char>(data[i]))) {
        ESP_LOGI(TAG, "Sentence: %s", line_);
        storeSentence();
      }
    }
  }

  // Add whatever functions or methods are needed.
  // if its just a helper function put it at the bottom of this file
  // if it should be a method of the class add it to the .hpp file
