#pragma once
#include "Wire.h"
#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"

class DriverBoardInterface {
    
    uint8_t _driver_board_i2c_address;

    bool _initialized = false;
  public:
    DriverBoardInterface(uint8_t driver_board_i2c_address) : _driver_board_i2c_address(driver_board_i2c_address) {}

    bool init()
    {
        Serial.println("Initializing I2C");
        Wire.begin();
        
        _initialized = true;
        return _initialized;
    }

    void sendCommand(const GroundCommand &command)
    {
        if (!_initialized) {
            Serial.println("[ERROR] [DriverBoardInterface]: I2C not initialized");
            return;
        }

        Wire.beginTransmission(_driver_board_i2c_address);
        Wire.write((uint8_t*)&command, sizeof(command));
        Wire.endTransmission(true);
    }

    bool requestState(SystemState &state_out)
    {
        if (!_initialized) {
            Serial.println("[ERROR] [DriverBoardInterface]: I2C not initialized");
            return false;
        }

        Wire.requestFrom(_driver_board_i2c_address, sizeof(SystemState));
        auto num_bytes = Wire.readBytes((uint8_t*)&state_out, sizeof(SystemState));
        if (num_bytes != sizeof(SystemState)) {
            Serial.println("[ERROR] [DriverBoardInterface]: Failed to read SystemState from I2C");
            return false;
        }
        return true;
    }

};

