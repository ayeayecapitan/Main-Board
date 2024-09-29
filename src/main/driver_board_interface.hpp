#pragma once
#include "Wire.h"

#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"
#include "shared/debug.hpp"



class DriverBoardInterface {
    
    uint8_t _driver_board_i2c_address;
    bool _initialized = false;

    int consecutive_request_failures = 0;
  public:
    DriverBoardInterface(uint8_t driver_board_i2c_address) : _driver_board_i2c_address(driver_board_i2c_address) {}

    bool init()
    {
        DEBUG_PRINTLN(F("Initializing I2C"));
        Wire.begin();


        _initialized = true;
        return _initialized;
    }

    void sendCommand(const GroundCommand &command)
    {
        if (!_initialized) {
            DEBUG_PRINTLN(F("[ERROR] [DriverBoardInterface]: I2C not initialized"));
            return;
        }

        Wire.beginTransmission(_driver_board_i2c_address);
        Wire.write((uint8_t*)&command, sizeof(command));
        Wire.endTransmission(true);
    }

    bool requestState(SystemState &state_out)
    {
        if (!_initialized) {
            DEBUG_PRINTLN(F("[ERROR] [DriverBoardInterface]: I2C not initialized"));
            return false;
        }

        SystemState state;
        auto num_bytes = Wire.requestFrom(_driver_board_i2c_address, sizeof(state));

        if (num_bytes == 0)
        {
            consecutive_request_failures++;
            if (consecutive_request_failures > 100) {
                DEBUG_PRINTLN(F("[ERROR] [DriverBoardInterface::requestState] - Driver board busy or unavailable"));
            }            
            return false;
        }

        if (num_bytes != sizeof(state)) {
            DEBUG_PRINT(F("[ERROR] [DriverBoardInterface::requestState]: Expected "));
            DEBUG_PRINT(sizeof(state));
            DEBUG_PRINT(" bytes got ");
            DEBUG_PRINTLN(num_bytes);
            return false;
        }

        num_bytes = Wire.readBytes((uint8_t*)&state, sizeof(state));
        if (num_bytes != sizeof(state)) {
            DEBUG_PRINTLN(F("[ERROR] [DriverBoardInterface::requestState]: Failed to read SystemState from I2C"));
            return false;
        }

        
        // Check if the received state is valid
        if (!state.signatureValid()) {
            DEBUG_PRINTLN(F("[ERROR] [DriverBoardInterface::requestState]: Data corrupted"));
            //print state as hex to serial
            auto *state_bytes = (uint8_t *)&state;
            for (size_t i = 0; i < sizeof(SystemState); i++)
            {

                // print leading zero if needed
                if (state_bytes[i] < 0x10)
                {
                    DEBUG_PRINT("0");
                }
                DEBUG_PRINT(state_bytes[i], HEX);
            }
            DEBUG_PRINTLN();
            return false;
        }

        //_sd_controller.log(state);

        state_out = state;

        consecutive_request_failures = 0;
        return true;
    }

};

