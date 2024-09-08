#pragma once

#include <Arduino.h>
#include "shared/data.hpp"

#include "hardware/valve.hpp"


class SpeProbe
{
    const probe::Descriptor &_descriptor;
    Valve _inlet { _descriptor.inlet_valve_index };
    Valve _outlet { _descriptor.outlet_valve_index };

    bool _probing = false;

    public:
        SpeProbe (uint8_t probe_index) : _descriptor(probe::descriptor[probe_index])
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
            _probing = true;
        }

        void stop()
        {
            disablePump();
            _outlet.close();
            _inlet.close();
            _probing = false;
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
}

