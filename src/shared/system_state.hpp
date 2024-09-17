#pragma once
#include "data.hpp"

struct SystemState
{
    uint64_t timestamp_us = 0;

    struct GpsData {
        float altitude = 0;
        float latitude = 0;
        float longitude = 0;
        uint8_t satellites = 0;

        operator String() const;
    } gps;

    struct SensorsData {
        float temperatures_c[temperature::COUNT] = {0};
        float pressure_pa = 0;
        float voltage = 0;
        float current = 0;
        float power = 0;
        operator String() const;
    } sensors;

    struct Devices {
        valve::state valve_state[valve::COUNT] = { valve::state::UNSET };
        pump::state pump_state[pump::COUNT] = { pump::state::UNSET };

        operator String() const;
    } devices;

    // to String cast
    operator String() const;
};

SystemState::GpsData::operator String() const
{
    return "GPS\t\t[alt: " + String(altitude) + " m, lat: " + String(latitude) + ", lon: " + String(longitude) + ", sats: " + String(satellites) + "]"; 
}

SystemState::SensorsData::operator String() const
{
    String str = "TEMPERATURE\t[";
    for (size_t i = 0; i < temperature::COUNT; i++)
    {
        str += temperatures_c[i];
        str += " C";
        if (i < temperature::COUNT - 1)
            str += ", ";     
    }
    str += "\nPRESSURE\t" + String(pressure_pa) + " Pa";
    str += "\nVOLTAGE\t\t" + String(voltage) + " mV";
    str += "\nCURRENT\t\t" + String(current) + " mA";
    str += "\nPOWER\t\t" + String(power) + " mW";
    return str;
}

SystemState::Devices::operator String() const
{
    String str = "VALVES\t\t[";
    for (size_t i = 0; i < valve::COUNT; i++)
    {
        str += toString(valve_state[i]);
        if (i < valve::COUNT - 1)
            str += ", ";
    }
    str += "]\nPUMPS\t\t[";
    for (size_t i = 0; i < pump::COUNT; i++)
    {
        str += toString(pump_state[i]);
        if (i < pump::COUNT - 1)
            str += ", ";
    }
    str += "]";
    return str;
}

SystemState::operator String() const
{
    return String("\n\nSYSTEM STATE\n")
        + "timestamp\t" + toString(timestamp_us) + " us\n"
        + String(gps) + "\n"
        + String(sensors) + "\n"
        + String(devices) + "\n\n";
}


