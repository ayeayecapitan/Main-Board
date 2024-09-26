#pragma once
#include "Wire.h"
#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"

class MainBoardInterface {
private:
    bool _command_available = false;
    GroundCommand _command;

    SystemState _state;

    static MainBoardInterface *_instance;
public:
    MainBoardInterface() = default;

    void init(){
        Wire.begin(driver_board::I2C_ADDRESS);  // Initialize I2C bus as slave
        resume();
        _instance = this;
  
        Serial.print(F("I2C initialized as slave on address "));
        Serial.println(driver_board::I2C_ADDRESS, HEX);
    }

    void pause()
    {
        TWCR &= ~(_BV(TWEN));  // Clear TWEN (TWI Enable Bit), disabling I2C hardware
        Wire.onRequest(nullptr);
        Wire.onReceive(nullptr);
    }

    void resume()
    {
        TWCR |= _BV(TWEN);  // Set TWEN to enable I2C hardware again
        Wire.onRequest(&requestEvent);
        Wire.onReceive(&receiveEvent);
    }

    void setState(const SystemState& state) {
        _state = state;
    }

    static void requestEvent() {
        if(_instance == nullptr) {
            Serial.println(F("MainBoardInterface not initialized"));
            return;
        }
        Wire.write((uint8_t*)&_instance->_state, sizeof(_instance->_state));
        Serial.println("STATE -> MAIN");
    }

    static void receiveEvent(int numBytes) {
        if(_instance == nullptr) {
            Serial.println(F("MainBoardInterface not initialized"));
            return;
        }

        if(numBytes != sizeof(GroundCommand)){
            Serial.println(F("Invalid message size"));
            while(Wire.available() > 0) // empty the buffer
                Wire.read();
            return;
        }
        

        GroundCommand new_command;
        auto num_read = Wire.readBytes((uint8_t*)&new_command, sizeof(new_command));
        if(num_read != sizeof(new_command)){
            Serial.println(F("Error reading command"));
            return;
        }
        _instance->_command = new_command;
        _instance->_command_available = true;
    }

    bool commandAvailable(uint64_t last_timestamp) const 
    {
        return _command_available && _command.timestamp_us > last_timestamp;
    }
    const GroundCommand& latestCommand() const { return _command; }
};

MainBoardInterface* MainBoardInterface::_instance = nullptr;