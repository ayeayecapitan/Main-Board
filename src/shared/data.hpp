#pragma once
#include <Arduino.h>
#include <pins_arduino.h>

namespace valve
{
    constexpr uint8_t COUNT = 8;

    enum state : uint8_t
    {
        UNSET = 0,
        CLOSED,     // CLOSED endstop is ON, OPEN endstop is OFF
        OPENING,    // Both endstops are OFF, motor is ON with OPENING direction
        OPEN,       // OPEN endstop is ON, CLOSED endstop is OFF
        CLOSING,    // Both endstops are OFF, motor is ON with CLOSING direction
        INCOMPLETE, // Both endstops are OFF, motor is OFF
        ERROR       // Both endstops are ON, motor is OFF
    };

    namespace index
    {
        constexpr int NONE = -1;
        constexpr int PROBE_1_INLET = 0;
        constexpr int PROBE_1_OUTLET = 1;
        constexpr int PROBE_2_INLET = 2;
        constexpr int PROBE_2_OUTLET = 3;
        constexpr int PROBE_3_INLET = 4;
        constexpr int PROBE_3_OUTLET = 5;
        constexpr int PROBE_4_INLET = 6;
        constexpr int PROBE_4_OUTLET = 7;
    }
}

namespace probe {
    constexpr uint8_t COUNT = 4;

    namespace index {
        constexpr uint8_t PROBE_1 = 0;
        constexpr uint8_t PROBE_2 = 1;
        constexpr uint8_t PROBE_3 = 2;
        constexpr uint8_t PROBE_4 = 3;
    }

    enum class State : uint8_t
    {
        UNSET = 0,
        OFF,
        ON
    };

    struct Descriptor
    {
        uint8_t inlet_valve_index;
        uint8_t outlet_valve_index;

        // TODO: auto mode params
    };

    constexpr Descriptor descriptor[COUNT] = {
        {valve::index::PROBE_1_INLET, valve::index::PROBE_1_OUTLET},
        {valve::index::PROBE_2_INLET, valve::index::PROBE_2_OUTLET},
        {valve::index::PROBE_3_INLET, valve::index::PROBE_3_OUTLET},
        {valve::index::PROBE_4_INLET, valve::index::PROBE_4_OUTLET}
    };
}



namespace pump
{
    constexpr uint8_t COUNT = 2;
    
    enum class state : uint8_t
    {
        UNSET = 0,
        OFF,
        ON
    };

    namespace index
    {
        constexpr uint8_t SPE_PROBES = 0;
        constexpr uint8_t CHEM_PROBES = 1;
    }
}
namespace temperature {
    constexpr uint8_t COUNT = 8;
    namespace index
    {
        constexpr size_t DS18B20_1_OUTSIDE = 0;
        constexpr size_t DS18B20_2_OUTSIDE = 1;
        constexpr size_t DS18B20_3_OUTSIDE = 2;
        constexpr size_t DS18B20_4_OUTSIDE = 3;
        constexpr size_t DS18B20_5_INSIDE = 4;
        constexpr size_t PT100_1 = 5;
        constexpr size_t PT100_2 = 6;
        constexpr size_t HONEYWELL = 7;
    }
}


namespace driver_board{
    constexpr uint8_t I2C_ADDRESS = 0x8;
    constexpr uint32_t SERIAL_BAUD_RATE = 115200;

    namespace ds18b20 {
        constexpr uint8_t COUNT = 5;

        constexpr uint8_t PRECISION = 12; // TODO: Check if this is correct
        namespace one_wire{

            constexpr int PIN = 5;
            namespace index {
                constexpr uint8_t DS18B20_1_OUTSIDE = 0;
                constexpr uint8_t DS18B20_2_OUTSIDE = 1;
                constexpr uint8_t DS18B20_3_OUTSIDE = 2;
                constexpr uint8_t DS18B20_4_OUTSIDE = 3;
                constexpr uint8_t DS18B20_5_INSIDE = 4;
            }
        }
    }

    namespace honeywell {
        namespace i2c {
            constexpr uint8_t ADDRESS = 0x28;
        }
    }

    namespace ina260 {
        namespace i2c {
            constexpr uint8_t ADDRESS = 0x40;
        }
    }

    namespace pt100 {
        constexpr uint8_t COUNT = 2;

        constexpr float MAX_RAW_VALUE = 0xFFFF; // PT100 temperature = (reading / MAX_RAW_VALUE) * REFERENCE_RESISTANCE
        constexpr float REFERENCE_RESISTANCE = 430.0; // PT100 temperature = (reading / MAX_RAW_VALUE) * REFERENCE_RESISTANCE


        namespace spi
        {
            constexpr uint8_t PT100_1 = 40;
            constexpr uint8_t PT100_2 = 41;
        }

        namespace index
        {
            constexpr uint8_t PT100_1 = 0;
            constexpr uint8_t PT100_2 = 1;
        }
    }

    namespace gps {
        namespace i2c
        {
            constexpr uint8_t ADDRESS = 0x42;
        }
    }


    namespace heater {
        constexpr uint8_t PIN = 38;
    }


    namespace valve {
        constexpr int OPEN_CLOSE_TIMEOUT_MS = 2000;
        struct Descriptor
        {
            uint8_t motor_pin;
            uint8_t open_endstop_pin;
            uint8_t closed_endstop_pin;
        };

        constexpr Descriptor descriptor[::valve::COUNT] = {
            {12, 11, 10},  // V1_INLET
            {9 , 7 , 8 },  // V1_OUTLET
            {A0, 2 , 3 },  // V2_INLET
            {A1, 28, 29},  // V2_OUTLET
            {A2, 31, 30},  // V3_INLET
            {25, 33, 32},  // V3_OUTLET
            {26, 35, 34},  // V4_INLET
            {27, 37, 36}   // V4_OUTLET
        };

        namespace motor {
            
            namespace pin
            {
                constexpr uint8_t DIRECTION_ALL = 13;
            }

            enum class Direction : uint8_t
            {
                CLOSING = 0,
                OPENING = 1
            };
        }

    }

    namespace pump
    {
        namespace pin
        {
            constexpr uint8_t SPE_PROBES = 39;
            constexpr uint8_t CHEM_PROBES = 40;
        }
    }

}

namespace main_board{
    constexpr uint32_t SERIAL_BAUD_RATE = 9600;
}


String toString(uint64_t timestamp_us)
{
    // String() has no overload for uint64_t
    // we split the timestamp into two uint32_t
    // and concatenate them as strings
    uint32_t high = timestamp_us >> 32;
    uint32_t low = timestamp_us & 0xFFFFFFFF;

    if(high == 0) return String(low);
    return String(high) + String(low);
}

String toString(pump::state state)
{
    switch (state)
    {
        case pump::state::UNSET:
            return "UNSET";
        case pump::state::OFF:
            return "OFF";
        case pump::state::ON:
            return "ON";
        default:
            return "UNKNOWN";
    }
}


String toString(valve::state state)
{
    switch (state)
    {
        case valve::state::UNSET:
            return "UNSET";
        case valve::state::CLOSED:
            return "CLOSED";
        case valve::state::OPEN:
            return "OPEN";
        case valve::state::INCOMPLETE:
            return "PENDING";
        case valve::state::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}