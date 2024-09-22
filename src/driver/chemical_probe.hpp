#pragma once
#include <Arduino.h>
#include "shared/data.hpp"


class ChemicalProbe
{
    public:
    
        ChemicalProbe() = default;

        static void init()
        {
            pinMode(driver_board::pump::pin::CHEM_PROBES, OUTPUT);
        }

        static void start()
        {
            digitalWrite(driver_board::pump::pin::CHEM_PROBES, HIGH);
        }

        static void stop()
        {
            digitalWrite(driver_board::pump::pin::CHEM_PROBES, LOW);
        }

        static pump::State pumpState()
        {
            if (digitalRead(driver_board::pump::pin::CHEM_PROBES) == HIGH)
                return pump::State::ON;
            return pump::State::OFF;
        }
};
