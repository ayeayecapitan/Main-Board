#pragma once
#include "IPAddress.h"
#include <Arduino.h>

namespace network { namespace ground_station {
    constexpr uint8_t MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    const IPAddress IP = { 172, 16, 18, 182 };  
    unsigned int UDP_PORT = 2000;
} }

namespace network { namespace main_board {
    constexpr uint8_t MAC[] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };  // MAC address
    const IPAddress IP = { 172, 16, 18, 181 };  // IP address
    unsigned int UDP_PORT = 1000;
} }



