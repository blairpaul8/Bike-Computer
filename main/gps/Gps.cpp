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
#include <cstdio>   // added: snprintf
#include <cstdlib>  // added: strtod, strtoul, atoi
#include <cstring>
#include "esp_log.h"


using namespace gps;

static const char* TAG = "Gps";

// added: helper declarations, definitions are at the bottom of this file
static const size_t MAX_FIELDS = 20;
static bool verifyChecksum(const char* s);
static size_t splitFields(char* s, const char* fields[], size_t max_fields);
static double toDecimalDegrees(const char* value, const char* hemisphere);
static void formatUtc(const char* hhmmss, char* out, size_t out_size);

  Gps::Gps() {}

  bool Gps::update(uint32_t timeout_ms) { (void)timeout_ms; return false; }

  GpsFix Gps::getFix() const { return GpsFix(); }
  
  bool Gps::hasFix() const { return false; }

  // added: parses the stored GGA (time, lat, lon, E/W) and VTG (speed km/h)
  bool Gps::parseGpsSentence() {
    char buf[sizeof(gga_)];
    const char* f[MAX_FIELDS];

    // VTG: 1 course | 2 T | 3 course | 4 M | 5 knots | 6 N | 7 km/h | 8 K | 9 mode
    if (vtg_[0] != '\0') {
      std::strncpy(buf, vtg_, sizeof(buf) - 1);
      buf[sizeof(buf) - 1] = '\0';
      if (verifyChecksum(buf) && splitFields(buf, f, MAX_FIELDS) >= 8) {
        speed_kmh_ = f[7][0] ? std::strtod(f[7], nullptr) : 0.0;
      }
      vtg_[0] = '\0';  // used, so an old VTG isn't paired with the next GGA
    }

    // GGA: 1 time | 2 lat | 3 N/S | 4 lon | 5 E/W | 6 quality | 7 sats | 8 hdop | 9 alt
    std::strncpy(buf, gga_, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    if (!verifyChecksum(buf)) {
      ESP_LOGW(TAG, "GGA bad checksum");
      return false;
    }
    if (splitFields(buf, f, MAX_FIELDS) < 10) {
      return false;
    }

    formatUtc(f[1], utc_time_, sizeof(utc_time_));
    latitude_ = toDecimalDegrees(f[2], f[3]);
    longitude_ = toDecimalDegrees(f[4], f[5]);
    ew_ = f[5][0];

    ESP_LOGI(TAG, "UTC=%s lat=%.6f lon=%.6f %c speed=%.3f km/h",
             utc_time_, latitude_, longitude_, ew_ ? ew_ : '?', speed_kmh_);

    return (f[6][0] ? std::atoi(f[6]) : 0) > 0;
  }

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
      parseGpsSentence();  // added: VTG arrives before GGA, so both are ready now
    } else if (std::strncmp(line_ + 3, "VTG", 3) == 0) {  // added
      std::strncpy(vtg_, line_, sizeof(vtg_) - 1);
      vtg_[sizeof(vtg_) - 1] = '\0';
      ESP_LOGI(TAG, "VTG stored: %s", vtg_);
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

  // Checksum is the XOR of every char between '$' and '*',
  // compared to the 2 hex digits after '*'
  static bool verifyChecksum(const char* s) {
    if (s[0] != '$') return false;

    uint8_t calc = 0;
    const char* p = s + 1;
    while (*p && *p != '*') {
      calc ^= static_cast<uint8_t>(*p++);
    }
    if (*p != '*' || !p[1] || !p[2]) return false;

    char hex[3] = {p[1], p[2], '\0'};
    char* end = nullptr;
    unsigned long given = std::strtoul(hex, &end, 16);
    return *end == '\0' && given == calc;
  }

  // Splits IN PLACE: replaces commas (and '*') with '\0' so each field is its
  // own C string. Empty fields stay as "" so indices don't shift (strtok would
  // skip them). fields[0] is the header, e.g. "GPGGA".
  static size_t splitFields(char* s, const char* fields[], size_t max_fields) {
    char* star = std::strchr(s, '*');
    if (star) *star = '\0';

    size_t n = 0;
    fields[n++] = s + 1;  // skip '$'
    for (char* p = s + 1; *p && n < max_fields; p++) {
      if (*p == ',') {
        *p = '\0';
        fields[n++] = p + 1;
      }
    }
    return n;
  }

  // ddmm.mmmmm (lat) or dddmm.mmmmm (lon) -> signed decimal degrees
  static double toDecimalDegrees(const char* value, const char* hemisphere) {
    if (!value[0]) return 0.0;
    double raw = std::strtod(value, nullptr);
    int deg = static_cast<int>(raw / 100.0);
    double dd = deg + (raw - deg * 100.0) / 60.0;
    if (hemisphere[0] == 'S' || hemisphere[0] == 'W') dd = -dd;
    return dd;
  }

  // 142725.00 -> "14:27:25.00"
  static void formatUtc(const char* hhmmss, char* out, size_t out_size) {
    if (std::strlen(hhmmss) < 6) {
      out[0] = '\0';
      return;
    }
    std::snprintf(out, out_size, "%.2s:%.2s:%s", hhmmss, hhmmss + 2, hhmmss + 4);
  }
