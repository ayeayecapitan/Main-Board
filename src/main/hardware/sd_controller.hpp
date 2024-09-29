#pragma once
#include <Arduino.h>
#include <SD.h>

#include "pins_arduino.h"
#include "shared/data.hpp"
#include "shared/debug.hpp"
#include "shared/system_state.hpp"
#include "utility/SdFat.h"

#include <main/pins.hpp>

class SDController
{
    File _file;

public:
    SDController() = default;
    ~SDController() = default;

    // NO COPY AND MOVE
    SDController(const SDController&) = delete;
    SDController& operator=(const SDController&) = delete;
    SDController(SDController&&) = delete;
    SDController& operator=(SDController&&) = delete;

    void init()
    {
        setPinMode(pinout::digital::SD_CARD, OUTPUT); 
        // constexpr uint32_t SD_CLOCK = 1000000; // TODO: check if this is the right value
        if(!SD.begin(pinout::digital::SD_CARD))
        {
            DEBUG_PRINTLN("SD initialization failed");
            return;
        }
    }

    //Method to log a SystemState to the SD card
    void log(SystemState& state){
        _file = SD.open("1.txt", FILE_WRITE);
        if(!_file)
        {
            DEBUG_PRINTLN("Failed to open log file");
            return;
        }
  
        _file.write((const char*)&state, sizeof(state));
        _file.print("\n");
        if(_file.getWriteError() != 0)
        {
            DEBUG_PRINTLN("SD write error");
        }

        _file.close();
    }
};