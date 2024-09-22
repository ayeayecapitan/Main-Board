#pragma once

#include <Arduino.h>
#include "shared/data.hpp"

#include "hardware/valve.hpp"


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
        }

        void start()
        {
            _inlet.open();
            _outlet.open();
            enablePump();
            _active = true;
        }

        void stop()
        {
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

        void getValveStates(valve::state valve_state[valve::COUNT])
        {
            valve_state[_descriptor.inlet_valve_index] = _inlet.state();
            valve_state[_descriptor.outlet_valve_index] = _outlet.state();
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
