#pragma once
#include "data.hpp"

struct SystemState
{
    static constexpr uint16_t SIGNATURE = (uint16_t('M') << 8) + 'S';
    uint16_t begin_signature = SIGNATURE;

    uint64_t timestamp_us = 0;

    struct SensorsData {
        float temperatures_c[temperature::COUNT] = {0};
        float pressure_pa = 0;

        struct Gps
        {
            float altitude = 0;
            float latitude = 0;
            float longitude = 0;
            uint8_t satellites = 0;
            operator String() const;
        } gps;

        struct PowerSupply
        {
            float voltage = 0;
            float current = 0;
            float power = 0;
            operator String() const;
        } power_supply;

        operator String() const;
    } sensors;

    struct Devices {
        valve::state valves_state[valve::COUNT] = { valve::state::UNSET };
        pump::State spe_pump_state = pump::State::UNSET;
        pump::State chemical_pump_state = pump::State::UNSET;
        operator String() const;
    } devices;

    uint16_t end_signature = SIGNATURE;
    // to String cast
    operator String() const;

    bool signatureValid()
    {
        return begin_signature == SIGNATURE && end_signature == SIGNATURE;
    }
};

SystemState::SensorsData::Gps::operator String() const
{
    return "GPS\t\t[alt: " + String(altitude) + " m, lat: " + String(latitude) + ", lon: " + String(longitude) + ", sats: " + String(satellites) + "]"; 
}

SystemState::SensorsData::PowerSupply::operator String() const
{
    return "VOLTAGE\t\t" + String(voltage) + " mV\nCURRENT\t\t" + String(current) + " mA\nPOWER\t\t" + String(power) + " mW";
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
    str += "]\nPRESSURE\t" + String(pressure_pa) + " Pa\n";
    str += String(gps) + "\n";
    str += "\n" + String(power_supply) + "\n";
    return str;
}

SystemState::Devices::operator String() const
{
    String str = "VALVES\t\t[";
    for (size_t i = 0; i < valve::COUNT; i++)
    {
        str += toString(valves_state[i]);
        if (i < valve::COUNT - 1)
            str += ", ";
    }
    str += "] SPE PUMP\t" + toString(spe_pump_state) + " CHEMICAL PUMP\t" + toString(chemical_pump_state);
    return str;
}

SystemState::operator String() const
{
    return String("\n\nSYSTEM STATE\n")
        + "timestamp\t" + toString(timestamp_us) + " us\n"
        + String(sensors) + "\n"
        + String(devices) + "\n\n";
}


