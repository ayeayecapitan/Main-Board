#pragma once 

#include <Arduino.h>
#include "shared/data.hpp"
#include "shared/debug.hpp"

// #define SIMULATION

class Valve
{
    const driver_board::valve::Descriptor &_descriptor;
    bool _is_open = false;
    uint8_t _index;

    #if defined(SIMULATION)
    int _simulation_open_endstop = HIGH;
    int _simulation_closed_endstop = LOW;
    #endif


public:
    Valve(uint8_t index)
        : _descriptor(driver_board::valve::descriptor[index]) , _index(index)
    {
        if(index < 0 || index >= valve::COUNT)
        {
            DEBUG_PRINTLN("[Valve::Valve] Invalid index");
            return;
        }
    }

    ~Valve() = default;

    // NO COPY AND MOVE
    Valve(const Valve&) = delete;
    Valve& operator=(const Valve&) = delete;
    Valve(Valve&&) = delete;
    Valve& operator=(Valve&&) = delete;

    void init()
    {
        #if not defined(SIMULATION)
        pinMode(_descriptor.motor_pin, OUTPUT);
        pinMode(_descriptor.open_endstop_pin, INPUT_PULLUP);
        pinMode(_descriptor.closed_endstop_pin, INPUT_PULLUP);
        #else
        pinMode(_descriptor.motor_pin, OUTPUT);
        pinMode(_descriptor.open_endstop_pin, OUTPUT);
        pinMode(_descriptor.closed_endstop_pin, OUTPUT);
        digitalWrite(_descriptor.open_endstop_pin, HIGH);
        digitalWrite(_descriptor.closed_endstop_pin, LOW);
        #endif
    }

    bool isOpen() const { return _is_open; }

    void enableMotor() { digitalWrite(_descriptor.motor_pin, HIGH); }

    void disableMotor() { digitalWrite(_descriptor.motor_pin, LOW); }

    bool openEndstopOn() const
    {
        #if defined(SIMULATION)
        return _simulation_open_endstop == LOW;
        #endif
        
        return digitalRead(_descriptor.open_endstop_pin) == LOW;
    }

    bool closedEndstopOn() const {

        #if defined(SIMULATION)
        return _simulation_closed_endstop == LOW;
        #endif

        return digitalRead(_descriptor.closed_endstop_pin) == LOW;
    }


    valve::state state()
    {
        auto openEndstop = openEndstopOn();
        auto closedEndstop = closedEndstopOn();

        if(!openEndstop && closedEndstop)
            return valve::state::CLOSED;

        if(openEndstop && !closedEndstop)
            return valve::state::OPEN;

        if(!openEndstop && !closedEndstop)
            return valve::state::INCOMPLETE;

        if(openEndstop && closedEndstop)
            return valve::state::ERROR;

        return valve::state::UNSET;
    }


    // Try to open the valve, if it fails, close it.
    // Returns true if the valve has opened successfully.
    // Blocks until the valve is open or timeout.
    bool open()
    {
        // motor direction is set for all valves so this method should be blocking to avoid conflicts
        if(openEndstopOn())
        {
            DEBUG_PRINTLN("[Valve::open] Valve " + String(_index + 1) + " already open");
            return true;
        }

        auto start_time = millis();
        setMotorsDirection(driver_board::valve::motor::Direction::OPENING);
        enableMotor();

        #if not defined(SIMULATION)
        while(!openEndstopOn())
        {
            if(millis() - start_time > driver_board::valve::OPEN_CLOSE_TIMEOUT_MS)
            {
                DEBUG_PRINTLN("[Valve::open] Valve " + String(_index + 1) + " open timeout - closing");
                close();
                disableMotor();
                return false;
            }
        }
        #else
        _simulation_closed_endstop = HIGH;
        delay(1000);
        _simulation_open_endstop = LOW;
        #endif

        disableMotor();
        return true;
    }

    // Try to close the valve.
    // Returns true if the valve has closed successfully.
    bool close()
    {
        // motor direction is set for all valves so this method should be blocking to avoid conflicts
        if(closedEndstopOn())
        {
            DEBUG_PRINTLN("[Valve::close] Valve " + String(_index + 1) + " already closed");
            return false;
        }

        auto start_time = millis();
        setMotorsDirection(driver_board::valve::motor::Direction::CLOSING);
        enableMotor();

        #if not defined(SIMULATION)
        while(!closedEndstopOn())
        {
            if(millis() - start_time > driver_board::valve::OPEN_CLOSE_TIMEOUT_MS)
            {
                DEBUG_PRINTLN("[Valve::close] Valve " + String(_index + 1) + " close timeout");
                disableMotor();
                return false;
            }
        }
        #else
        _simulation_open_endstop = HIGH;
        delay(1000);
        _simulation_closed_endstop = LOW;
        #endif

        disableMotor();
        return true;
    }

    static void setMotorsDirection(driver_board::valve::motor::Direction direction) 
    {
        using namespace driver_board::valve;
        switch (direction)
        {
        case motor::Direction::CLOSING:
            digitalWrite(motor::pin::DIRECTION_ALL, LOW);
            break;
        case motor::Direction::OPENING:
            digitalWrite(motor::pin::DIRECTION_ALL, HIGH);
            break;
        default:
            DEBUG_PRINTLN("[Valve::setMotorDirection] Invalid direction, setting to CLOSING");
            digitalWrite(motor::pin::DIRECTION_ALL, LOW);
            break;
        }
    }
};