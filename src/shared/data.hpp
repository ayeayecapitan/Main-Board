#pragma once
#include <Arduino.h>

namespace driver_board{
    constexpr int I2C_ADDRESS = 0x8;
}

namespace valve
{
    constexpr uint8_t COUNT = 8;

    namespace command
    {
        
    }

    enum state : uint8_t
    {
        UNSET = 0,
        CLOSED,
        OPEN,
        PENDING, // Obie krańcówki są w stanie OFF
        ERROR   // Obie krańcówki są w stanie ON (TODO: pomyśleć nad innymi przypadkami)
    };

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

    namespace driver_board
    {
        namespace pin
        {
            //First pair
            constexpr int V1_INLET_OPENED = 11;
            constexpr int V1_OUTLET_CLOSED = 10;

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
        //Endstops paired in [Endstop, valve]-[SPE column]-[Endstop, valve]
    }


}

namespace motor
{
    constexpr uint8_t COUNT = 8;

    enum class Direction : uint8_t
    {
        UNSET = 0,
        OPENING,
        BACKWARD
    };

    enum class state : uint8_t
    {
        UNSET = 0,
        OFF,
        OPENING,
        CLOSING
    };

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

    namespace driver_board
    {
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
    }
}

namespace pump
{
    constexpr uint8_t COUNT = 2;
    
    enum class state : uint8_t
    {
        OFF,
        ON
    };
       
    namespace index
    {
        constexpr uint8_t P_SPE = 0;
        constexpr uint8_t P_CHEM = 1;
    }

    namespace driver_board
    {
        namespace pin
        {
            constexpr int P_SPE = 0;
            constexpr int P_CHEM = 1;
        }
    }
}

namespace pressure {
    namespace i2c {
        constexpr uint8_t HONEYWELL = 0x28;
    }
}

namespace temperature {
    constexpr uint8_t COUNT = 5;

    namespace index
    {
        constexpr uint8_t DS18B20_1 = 0;
        constexpr uint8_t DS18B20_2 = 1;
        constexpr uint8_t PT100_1 = 2;
        constexpr uint8_t PT100_2 = 3;
        constexpr uint8_t HONEYWELL = 4;
    }
    
    namespace driver_board
    {
        namespace spi
        {
            constexpr int PT100_1 = 0;
            constexpr int PT100_2 = 1;
        }

        namespace i2c
        {
            constexpr uint8_t HONEYWELL = pressure::i2c::HONEYWELL;
        }
    }
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

struct SystemState
{
    uint64_t timestamp_ns = 0;

    struct {
        float altitude = 0;
        float latitude = 0;
        float longitude = 0;
        uint8_t satellites = 0;
    } gps;

    struct {
        float pressure = 0;
        float temperature[temperature::COUNT] = {0};
    } sensors;

    struct {
        float voltage = 0;
        float current = 0;
        float power = 0;
    } supply;

    struct {
        motor::Direction motors_direction = motor::Direction::UNSET;
        uint8_t motor_state[motor::COUNT] = {0};
        uint8_t valve_state[valve::COUNT] = {0};
        uint8_t pump_state[pump::COUNT] = {0};
    } devices;
};