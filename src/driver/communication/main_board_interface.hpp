#pragma once
#include "Wire.h"
#include "shared/data.hpp"

class MainBoardInterface {
private:
    uint8_t _address;


    bool _command_available = false;
    GcsCommand _command;

    static MainBoardInterface *_instance;
public:
    MainBoardInterface(uint8_t addr) : _address(addr) {}

    void init(){
        Wire.begin(_address);  // Initialize I2C bus as slave
        resume();
        _instance = this;
  
        Serial.print("I2C initialized as slave on address ");
        Serial.println(_address, HEX);
    }

    void pause()
    {
        Wire.onRequest(nullptr);
        Wire.onReceive(nullptr);
    }

    void resume()
    {
        Wire.onRequest(&requestEvent);
        Wire.onReceive(&receiveEvent);
    }

    static void requestEvent() {

        
    }

    static void receiveEvent(int numBytes) {
        if(_instance == nullptr) {
            Serial.println("MainBoardInterface not initialized");
            return;
        }

        if(numBytes != sizeof(GcsCommand)){
            Serial.println("Invalid message size");
            while(Wire.available() > 0) // empty the buffer
                Wire.read();
            return;
        }

        GcsCommand new_command;
        auto num_read = Wire.readBytes((uint8_t*)&new_command, sizeof(new_command));
        if(num_read != sizeof(new_command)){
            Serial.println("Error reading command");
            return;
        }
        _instance->_command = new_command;
        _instance->_command_available = true;
    }

    bool commandAvailable() const { return _command_available; }
    const GcsCommand& latestCommand() const { return _command; }
};

MainBoardInterface* MainBoardInterface::_instance = nullptr;