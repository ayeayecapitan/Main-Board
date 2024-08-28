#pragma once
#include <Adafruit_I2CDevice.h>


class I2CMaster {
    Adafruit_I2CDevice i2c;

  public:
    I2CMaster(uint8_t slave_address) : i2c(slave_address) {

        // Initialize I2C as Master
        Wire.begin();

        if (!i2c.begin()) {
            Serial.print("Did not find device at 0x");
            Serial.println(i2c.address(), HEX);
        }
    }

    void sendMessage(const char *message) {
        i2c.begin();
        i2c.write((uint8_t *)message, strlen(message) + 1);  // Send message with null terminator
        i2c.end();
    }
};
