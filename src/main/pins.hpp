#pragma once

namespace pinout {
    namespace digital {
        constexpr int LED = 13;
        constexpr int ETHERNET = 5;
        constexpr int SD_CARD = 7;

        //HEATER
        constexpr int HEATER1 = 38;
        constexpr int HEATER2 = 39;

        //PUMP
        constexpr int PUMP1 = 42;
        constexpr int PUMP2 = 49;

        namespace temperature {
            constexpr int DS18B20_1 = 9;
            constexpr int DS18B20_2 = 5;
            constexpr int PT100_1 = 40;
            constexpr int PT100_2 = 41;
        }

        //MOTOR
        constexpr int MOTOR_DIRECTION = 13;

        constexpr int MOTOR1 = 12;
        constexpr int MOTOR2 = 9;
        constexpr int MOTOR3 = 0;
        constexpr int MOTOR4 = 1;
        constexpr int MOTOR5 = 2;
        constexpr int MOTOR6 = 25;
        constexpr int MOTOR7 = 26;
        constexpr int MOTOR8 = 27;

        //VALVE
        constexpr int VALVE_OPEN_1 = 11;
        constexpr int VALVE_CLOSED_1 = 10;
        constexpr int VALVE_OPEN_2 = 7;
        constexpr int VALVE_CLOSED_2 = 8;
        constexpr int VALVE_OPEN_3 = 2;
        constexpr int VALVE_CLOSED_3 = 3;
        constexpr int VALVE_OPEN_4 = 28;
        constexpr int VALVE_CLOSED_4 = 29;
        constexpr int VALVE_OPEN_5 = 31;
        constexpr int VALVE_CLOSED_5 = 30;
        constexpr int VALVE_OPEN_6 = 33;
        constexpr int VALVE_CLOSED_6 = 32;
        constexpr int VALVE_OPEN_7 = 35;
        constexpr int VALVE_CLOSED_7 = 34;
        constexpr int VALVE_OPEN_8 = 37;
        constexpr int VALVE_CLOSED_8 = 36;
    }
}