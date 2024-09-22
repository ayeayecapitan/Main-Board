#pragma once
#include "shared/data.hpp"
#include <Arduino.h>
#include <SD.h>

class SDcard {
    public:
        SDcard() = default;

        void init(){
            Serial.println("SD card initialized");
        }

        void update()
        {
            Serial.println("SD card updated");
        }

        void readTemperatureC(uint8_t one_wire_index)
        {
            Serial.println("SD card read");
        }
};