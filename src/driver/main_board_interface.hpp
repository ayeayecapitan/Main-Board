#pragma once
#include "Spi.h"
#include "Wire.h"

class DriverBoardInterface {
private:
    uint8_t address;

public:
    DriverBoardInterface(uint8_t addr) : address(addr) {}

    void init(){
        Wire.begin(address);  // Initialize I2C bus as slave

        Wire.onRequest(&requestEvent);  // Register request event handler
        Wire.onReceive(&receiveEvent);  // Register receive event handler

        Serial.println("I2C slave initialized");
    }

    static void requestEvent() {
        
    }

    static void receiveEvent(int numBytes) {

        struct {
            uint8_t x = 0;
            uint32_t y = 0;
            bool z = 0;
            double w = 0.0;
        } data;

        Serial.print("Size of double: ");
        Serial.println(sizeof(data));

        if(numBytes != sizeof(data)){
            Serial.println("Invalid message size");
            return;
        }

        Wire.readBytes((uint8_t*)&data, sizeof(data));

        Serial.println("Received message:");
        Serial.print("x: ");
        Serial.println(data.x);
        Serial.print("y: ");
        Serial.println(data.y);
        Serial.print("z: ");
        Serial.println(data.z);
        Serial.print("w: ");
        Serial.println(data.w);


    }
};