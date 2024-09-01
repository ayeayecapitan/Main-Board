#pragma once
#include <Arduino.h>

namespace valve
{
    namespace command {
        
    }

    namespace state
    {
        constexpr uint8_t CLOSED = 0;
        constexpr uint8_t OPEN = 1;
        constexpr uint8_t PENDING = 2; // Obie krańcówki są w stanie OFF
        constexpr uint8_t ERROR = 3;   // Obie krańcówki są w stanie ON (TODO: pomyśleć nad innymi przypadkami)
    }

    namespace index
    {
        constexpr uint8_t V1_INLET = 0;
        constexpr uint8_t V1_OUTLET = 1;
        constexpr uint8_t V2_INLET = 2;
        constexpr uint8_t V2_OUTLET = 3;
        constexpr uint8_t V3_INLET = 4;
        constexpr uint8_t V3_OUTLET = 5;
        constexpr uint8_t V4_INLET = 6;
        constexpr uint8_t V4_OUTLET = 7;
    }

    namespace motor
    {



    }

    namespace pin
    {
        //Endstops paired in [Endstop, valve]-[SPE column]-[Endstop, valve]
        
        //First pair
        constexpr int V1_INLET_OPENED = 0;
        constexpr int V1_OUTLET_CLOSED = 0;

        //Second pair
        constexpr int V2_INLET_OPENED = 0;
        constexpr int V2_OUTLET_CLOSED = 0;

        //Third pair
        constexpr int V3_INLET_OPENED = 0;
        constexpr int V3_OUTLET_CLOSED = 0;
        
        //Fourth pair
        constexpr int V4_INLET_OPENED = 6;
        constexpr int V4_OUTLET_CLOSED = 7;
    }

    constexpr uint8_t COUNT = 8;
}

namespace motor
{
    namespace state
    {
        constexpr uint8_t OFF = 0;
        constexpr uint8_t OPENING = 1;
        constexpr uint8_t CLOSING = 2;
    }

    namespace index
    {
        constexpr uint8_t M1_INLET = 0;
        constexpr uint8_t M1_OUTLET = 1;
        constexpr uint8_t M2_INLET = 2;
        constexpr uint8_t M2_OUTLET = 3;
        constexpr uint8_t M3_INLET = 4;
        constexpr uint8_t M3_OUTLET = 5;
        constexpr uint8_t M4_INLET = 6;
        constexpr uint8_t M4_OUTLET = 7;
    }

    namespace pin
    {
        constexpr int M1_INLET = 0;
        constexpr int M1_OUTLET = 1;
        constexpr int M2_INLET = 2;
        constexpr int M2_OUTLET = 3;
        constexpr int M3_INLET = 4;
        constexpr int M3_OUTLET = 5;
        constexpr int M4_INLET = 6;
        constexpr int M4_OUTLET = 7;
    }

    constexpr uint8_t COUNT = 8;
}

namespace pump
{
    namespace status
    {
        constexpr uint8_t ON = 0;
        constexpr uint8_t OFF = 1;
    }

    namespace index
    {
        constexpr uint8_t P_SPE = 0;
        constexpr uint8_t P_CHEM = 1;
    }

    namespace pin
    {
        constexpr int P_SPE = 0;
        constexpr int P_CHEM = 1;
    }

    constexpr uint8_t COUNT = 2;
}

namespace pressure {
    namespace i2c {
        constexpr uint8_t HONEYWELL = 0x28;
    }
}

namespace temperature {
    
    namespace index
    {
        constexpr uint8_t DS18B20_1 = 0;
        constexpr uint8_t DS18B20_2 = 1;
        constexpr uint8_t PT100_1 = 2;
        constexpr uint8_t PT100_2 = 3;
        constexpr uint8_t HONEYWELL = 4;
    }
    
    namespace spi
    {
        constexpr int PT100_1 = 0;
        constexpr int PT100_2 = 1;
    }

    namespace i2c
    {
        constexpr uint8_t HONEYWELL = pressure::i2c::HONEYWELL;
    }

    constexpr uint8_t COUNT = 5;
}

namespace gps {
    namespace i2c
    {
        constexpr uint8_t GPS = 0x42;
    }
}

namespace heater {
    namespace pin {
        constexpr int HEATER = 38;
    }
}



// Command from Ground Control Station
struct GcsCommand
{
    uint64_t timestamp_ns;

    uint8_t x,y;

    // uint8_t motor_desired_status[motor::COUNT];
    // uint8_t valve_desired_status[valve::COUNT];
    // uint8_t pump_desired_status[pump::COUNT];

    // When auto_mode is true, the board should ignore the motor_desired_status, valve_desired_status and pump_desired_status fields
    // bool auto_mode;
};

struct GpsState
{
    uint8_t satellite_numbers;
    float altitude;
    float latitude;
    float longitude;
};

struct SensorState
{

};

struct MainBoardState
{
    uint64_t timestamp_ns;

    float altitude = 0;
    float latitude = 0;
    float longitude = 0;
    uint8_t satellite_numbers = 0;
    uint8_t temperature;
};