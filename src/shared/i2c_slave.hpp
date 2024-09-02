#pragma once
#include "Spi.h"
#include "Wire.h"

class I2CSlave {
private:
    uint8_t address;

public:
    I2CSlave(uint8_t addr) : address(addr) {}

    void init(){
        Wire.begin(address);  // Initialize I2C bus as slave
        Wire.onReceive(&receiveEvent);  // Register receive event handler

        Serial.println("I2C slave initialized");
    }

    static void receiveEvent(int numBytes) {
        Serial.print("Received message of length ");
        Serial.println(numBytes);
        char message[32];
        int i = 0;

        while (Wire.available() > 0 && i < sizeof(message) - 1) {
            message[i++] = Wire.read();  // Read each byte
        }
        message[i] = '\0';  // Null terminate the string

        Serial.print("Received message: ");
        Serial.println(message);
    }
};