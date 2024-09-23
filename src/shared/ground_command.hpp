#pragma once
#include <Arduino.h>
#include "data.hpp"

struct GroundCommand
{
    uint64_t timestamp_us = 0;
    probe::State probe_desired_state[probe::COUNT];

    operator String() const
    {
        String str = "GCS Command\ttimestamp: " + toString(timestamp_us) + " us, Probes: ";
        for (size_t i = 0; i < probe::COUNT; i++)
        {
            switch (probe_desired_state[i])
            {
            case probe::State::ON:
                str += "ON";
                break;
            case probe::State::OFF:
                str += "OFF";
                break;
            default:
                str += "UNS";
                break;
            }
            if (i < probe::COUNT - 1)
                str += ", ";
        }
        return str + "]";
    }
};