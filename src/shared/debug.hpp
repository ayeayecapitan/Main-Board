#pragma once
#include <Arduino.h>

// #define DEBUG_PRINT_ENABLE

// #if defined(DEBUG_PRINT_ENABLE)
//   #define DEBUG_PRINT(...)    Serial.print(__VA_ARGS__)
//   #define DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__)
// #else
//   #define DEBUG_PRINT(...)    // Do nothing
//   #define DEBUG_PRINTLN(...)  // Do nothing
// #endif


constexpr bool DEBUG_PRINT_ENABLED = true;

template <typename... Args>
void DEBUG_PRINT(Args... args) {
  if (DEBUG_PRINT_ENABLED) {
    Serial.print(args...);
  }
}

template <typename... Args>
void DEBUG_PRINTLN(Args... args) {
  if (DEBUG_PRINT_ENABLED) {
    Serial.println(args...);
  }
}
