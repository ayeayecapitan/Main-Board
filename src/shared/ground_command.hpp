#pragma once
#include <Arduino.h>
#include "data.hpp"

struct GroundCommand
{
    uint64_t timestamp_us = 0;
    probe::State probe_desired_state[probe::COUNT];

    operator String() const
    {
        String str = "GCS Command\t[timestamp: " + toString(timestamp_us) + " us]\n";
        str += "Probe states\t[";
        for (size_t i = 0; i < probe::COUNT; i++)
        {
            str += (uint8_t)probe_desired_state[i];
            if (i < probe::COUNT - 1)
                str += ", ";
        }
        return str + "]";
    }
};