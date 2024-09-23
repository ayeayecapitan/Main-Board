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

uint64_t last_state_request = 0;

void loop() {
    GroundStationInterface.processIncomingData();

    if(GroundStationInterface.commandAvailable(command.timestamp_us))
    {
        command = GroundStationInterface.latestCommand();
        // Serial.println(command);
        driver_board_interface.sendCommand(command);
        // Serial.println(F("GCS CMD -> DRIVER"));
    }
    
    if(millis() - last_state_request < 2000)
        return;
    last_state_request = millis();
    SystemState state;
    if(driver_board_interface.requestState(state))
    {
        Serial.println(state.devices);
        GroundStationInterface.sendState(state);
        // Serial.println(F("STATE -> GCS"));
    }

    // delay(1000);
}

//TODO:
//1. Downlink
//2. Uplink
//4. Frame signatures
//5. Telecommands
//6. Files
//8. Errr codes