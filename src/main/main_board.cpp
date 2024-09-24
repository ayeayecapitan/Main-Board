#include <Arduino.h>
#include <Ethernet.h>

#include "driver_board_interface.hpp"
#include "ground_station_interface.hpp"

#include "shared/data.hpp"
#include "shared/ground_command.hpp"

DriverBoardInterface driver_board_interface(driver_board::I2C_ADDRESS);
GroundStationInterface ground_station_interface;

GroundCommand command;
uint64_t last_state_request = 0;

void setup() {
    Serial.begin(main_board::SERIAL_BAUD_RATE);
    delay(1500); // Workaround for the serial monitor permissions issue - see after_upload in env.py

    ground_station_interface.init();
    driver_board_interface.init();
}

void loop() {
    // Process incoming UDP data from the ground station
    ground_station_interface.processIncomingData();

    // If new command is available forward it to the driver board
    if(ground_station_interface.commandAvailable(command.timestamp_us))
    {
        command = ground_station_interface.latestCommand();
        driver_board_interface.sendCommand(command);
        // Serial.println(command);
        // Serial.println(F("GCS CMD -> DRIVER"));
    }
    

    // Do every STATE_REQUEST_INTERVAL_MS
    constexpr uint64_t STATE_REQUEST_INTERVAL_MS = 1000;
    if(millis() - last_state_request >= STATE_REQUEST_INTERVAL_MS)
    {
        // Request the state from the driver board and forward it to the ground station
        SystemState state;
        if(driver_board_interface.requestState(state))
        {
            last_state_request = millis();
            ground_station_interface.sendState(state);
            Serial.println(state.devices);
            // Serial.println(F("STATE -> GCS"));
        }
    }

    // TODO save state to SD card
}