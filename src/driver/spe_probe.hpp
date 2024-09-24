#pragma once

#include <Arduino.h>
#include "shared/data.hpp"

#include "valve.hpp"


class SpeProbe
{
    const probe::Descriptor &_descriptor;

    uint8_t _probe_index;
    Valve _inlet { _descriptor.inlet_valve_index };
    Valve _outlet { _descriptor.outlet_valve_index };

    bool _active = false;

    public:
        SpeProbe (uint8_t probe_index) : _descriptor(probe::descriptor[probe_index]), _probe_index(probe_index)
        {
        }

        ~SpeProbe() = default;	
        // NO COPY AND MOVE
        SpeProbe(const SpeProbe&) = delete;
        SpeProbe& operator=(const SpeProbe&) = delete;
        SpeProbe(SpeProbe&&) = delete;
        SpeProbe& operator=(SpeProbe&&) = delete;

        void init()
        {
            pinMode(driver_board::pump::pin::SPE_PROBES, OUTPUT);
            _inlet.init();
            _outlet.init();

            //Check if state needs to be restored after reboot
            auto inlet_state = _inlet.state();
            auto outlet_state = _outlet.state();

            if(inlet_state == valve::state::OPEN && outlet_state == valve::state::OPEN)
            {
                Serial.println("[SpeProbe::init] Probe " + String(_probe_index + 1) + " was active before reboot");
                start();
            }

            //If one of the valves is open then close the valve, not sure if this is necessary
            if((inlet_state == valve::state::OPEN) != (outlet_state == valve::state::OPEN))
            {
                Serial.println("[SpeProbe::init] Probe " + String(_probe_index + 1) + " has an inconsistent state");
                stop();
            }
        }

        void start()
        {
            if(_active)
            {
                Serial.println("[SpeProbe::start] Probe " + String(_probe_index + 1) + " is already active");
                return;
            }

            if(!_inlet.open())
                return;
            if(!_outlet.open())
                return;
            enablePump();
            _active = true;
        }

        void stop()
        {
            if(!_active)
            {
                Serial.println("[SpeProbe::stop] Probe " + String(_probe_index + 1) + " is not active");
                return;
            }

            disablePump();
            _outlet.close();
            _inlet.close();
            _active = false;
        }

        uint8_t index() const
        {
            return _probe_index;
        }

        bool isActive() const
        {
            return _active;
        }

        void getValveStates(valve::state valves_state[valve::COUNT])
        {
            valves_state[_descriptor.inlet_valve_index] = _inlet.state();
            valves_state[_descriptor.outlet_valve_index] = _outlet.state();
        }

        static pump::State pumpState()
        {
            if(digitalRead(driver_board::pump::pin::SPE_PROBES) == HIGH)
                return pump::State::ON;
            return pump::State::OFF;
        }

    private:
        static void enablePump()
        {
            digitalWrite(driver_board::pump::pin::SPE_PROBES, HIGH);
        }

        static void disablePump()
        {
            digitalWrite(driver_board::pump::pin::SPE_PROBES, LOW);
        }
};
