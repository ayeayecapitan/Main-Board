#pragma once
#include "Spi.h"
#include "Wire.h"

class I2CMaster {
    
    uint8_t _slave_address;

    bool _initialized = false;

  public:
    I2CMaster(uint8_t slave_address) : _slave_address(slave_address) {}

    bool init()
    {
        Serial.println("Initializing I2C");
        Wire.begin();
        
        _initialized = true;
        return _initialized;
    }

    void sendMessage(const char *message) {
        if (!_initialized) {
            Serial.println("I2C not initialized");
            return;
        }

        Serial.println("Begin transmission");
        Wire.beginTransmission(_slave_address);
        Serial.println("Sending message");
        Wire.write(message);
        Serial.println("End transmission");
        Wire.endTransmission(true);
    }
};
