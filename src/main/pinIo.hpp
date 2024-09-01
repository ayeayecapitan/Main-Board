#pragma once
#include <Arduino.h>

namespace pin
{
    inline void digital_write(uint8_t pin, bool value)
    {
        digitalWrite(pin, (uint8_t) value);
    }
} // namespace pin
