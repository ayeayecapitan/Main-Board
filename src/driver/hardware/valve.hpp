#pragma once 

#include <Arduino.h>
#include "shared/data.hpp"

class Valve
{
    const driver_board::valve::Descriptor &_descriptor;
    bool _is_open = false;

public:
    Valve(uint8_t index)
        : _descriptor(driver_board::valve::descriptor[index])
    {
        if(index < 0 || index >= valve::COUNT)
        {
            Serial.println("[Valve::Valve] Invalid index");
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
        pinMode(_descriptor.motor_pin, OUTPUT);
        pinMode(_descriptor.open_endstop_pin, INPUT_PULLUP);
        pinMode(_descriptor.closed_endstop_pin, INPUT_PULLUP);
    }

    bool isOpen() const { return _is_open; }

    void enableMotor() { digitalWrite(_descriptor.motor_pin, HIGH); }

    void disableMotor() { digitalWrite(_descriptor.motor_pin, LOW); }

    bool openEndstopHigh() const { return digitalRead(_descriptor.open_endstop_pin) == HIGH; }

    bool closedEndstopHigh() const { return digitalRead(_descriptor.closed_endstop_pin) == HIGH; }


    void open()
    {
        // motor direction is set for all valves so this method should be blocking to avoid conflicts
        if(openEndstopHigh())
        {
            Serial.println("[Valve::open] Already open");
        }

        auto start_time = millis();
        setMotorsDirection(driver_board::valve::motor::Direction::OPENING);
        enableMotor();

        while(!openEndstopHigh())
        {
            if(millis() - start_time > driver_board::valve::OPEN_CLOSE_TIMEOUT_MS)
            {
                Serial.println("[Valve::open] Valve open timeout - closing");
                close();
                break;
            }
        }
        disableMotor();

        _is_open = true;
    }

    void close()
    {
        // motor direction is set for all valves so this method should be blocking to avoid conflicts
        if(closedEndstopHigh())
        {
            Serial.println("[Valve::close] Already closed");
            return;
        }

        auto start_time = millis();
        setMotorsDirection(driver_board::valve::motor::Direction::CLOSING);
        enableMotor();

        while(!closedEndstopHigh())
        {
            if(millis() - start_time > driver_board::valve::OPEN_CLOSE_TIMEOUT_MS)
            {
                Serial.println("[Valve::close] Valve close timeout");
                break;
            }
        }
        disableMotor();

        _is_open = false;
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
            Serial.println("[Valve::setMotorDirection] Invalid direction, setting to CLOSING");
            digitalWrite(motor::pin::DIRECTION_ALL, LOW);
            break;
        }
    }
};