#include <Arduino.h>
#include <Ethernet.h>

#include "driver_board_interface.hpp"
#include "ground_station_interface.hpp"

#include "shared/data.hpp"
#include "shared/ground_command.hpp"

DriverBoardInterface driver_board_interface(driver_board::I2C_ADDRESS); // NOLINT - global variable

void setup() {
    delay(1000); // wait for the monitor serial port to be available

    Serial.begin(main_board::SERIAL_BAUD_RATE);

    GroundStationInterface.init();
    driver_board_interface.init();
}

GroundCommand command;

void loop() {
    GroundStationInterface.processIncomingData();

    if(GroundStationInterface.commandAvailable(command.timestamp_us))
    {
        command = GroundStationInterface.latestCommand();

        Serial.println("RECEIVED COMMAND FROM GROUND STATION");
        Serial.println(command);
        driver_board_interface.sendCommand(command);
        Serial.println("FORWARDED COMMAND TO DRIVER BOARD");
    }
    
    SystemState state;
    if(driver_board_interface.requestState(state))
    {
        Serial.println("RECEIVED STATE FROM DRIVER BOARD");
        Serial.println(state);
        GroundStationInterface.sendState(state);
        Serial.println("FORWARDED STATE TO GROUND STATION");
    }
}

//TODO:
//1. Downlink
//2. Uplink
//4. Frame signatures
//5. Telecommands
//6. Files
//8. Errr codes