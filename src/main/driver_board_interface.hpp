#pragma once
#include "Wire.h"

#include "shared/data.hpp"
#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"
#include "shared/debug.hpp"



class DriverBoardInterface {
    bool _initialized = false;

    int consecutive_request_failures = 0;
  public:
    DriverBoardInterface() = default;

    bool init()
    {
        DEBUG_PRINTLN(F("DBI Init I2C"));
        Wire.begin();


        _initialized = true;
        return _initialized;
    }

    void sendCommand(const GroundCommand &command)
    {
        if (!_initialized) {
            DEBUG_PRINTLN(F("[sendCommand]: I2C not initialized"));
            return;
        }

        Wire.beginTransmission(driver_board::I2C_ADDRESS);
        Wire.write((uint8_t*)&command, sizeof(command));
        Wire.endTransmission(true);
    }

    bool requestState(SystemState &state_out)
    {
        if (!_initialized) {
            DEBUG_PRINTLN(F("[requestState]: I2C not initialized"));
            return false;
        }

        SystemState state;
        auto num_bytes = Wire.requestFrom(driver_board::I2C_ADDRESS, sizeof(state));

        if (num_bytes == 0)
        {
            consecutive_request_failures++;
            if (consecutive_request_failures > 100) {
                consecutive_request_failures = 0;
                DEBUG_PRINTLN(F("[requestState] - Driver board busy or unavailable"));
            }            
            return false;
        }

        if (num_bytes != sizeof(state)) {
            DEBUG_PRINT(F("[requestState]: Expected "));
            DEBUG_PRINT(sizeof(state));
            DEBUG_PRINT(F(" got "));
            DEBUG_PRINTLN(num_bytes);
            return false;
        }

        num_bytes = Wire.readBytes((uint8_t*)&state, sizeof(state));
        if (num_bytes != sizeof(state)) {
            DEBUG_PRINTLN(F("[requestState]: Failed to read from I2C"));
            return false;
        }

        
        // Check if the received state is valid
        if (!state.signatureValid()) {
            DEBUG_PRINTLN(F("[requestState]: Data corrupted"));
            //print state as hex to serial
            if(DEBUG_PRINT_ENABLED)
            {
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
            }
            return false;
        }
        consecutive_request_failures = 0;

        state_out = state;
        return true;
    }

};

