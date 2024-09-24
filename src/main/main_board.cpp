#include <Arduino.h>
#include <Ethernet.h>

#include "driver_board_interface.hpp"
#include "ground_station_interface.hpp"

#include "shared/data.hpp"
#include "shared/ground_command.hpp"

#include <avr/wdt.h>

#include <EEPROM.h>


DriverBoardInterface driver_board_interface(driver_board::I2C_ADDRESS);
GroundStationInterface ground_station_interface;

GroundCommand command;
uint64_t last_state_request = 0;

void enableWatchdog()
{
    delay(5000);
    wdt_enable(WDTO_8S);
    wdt_reset();
}

void setup() {
    // enableWatchdog(); TODO Enable watchdog
    Serial.begin(main_board::SERIAL_BAUD_RATE);
    delay(1500); // Workaround for the serial monitor permissions issue - see after_upload in env.py

    ground_station_interface.init();
    driver_board_interface.init();
}

void loop() {
    wdt_reset();
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
    constexpr uint64_t STATE_REQUEST_INTERVAL_MS = 250;
    if(millis() - last_state_request >= STATE_REQUEST_INTERVAL_MS)
    {
        // Request the state from the driver board and forward it to the ground station
        last_state_request = millis();
        SystemState state;
        if(driver_board_interface.requestState(state))
        {
            ground_station_interface.sendState(state);
            // Serial.println(state.devices);
            Serial.println(F("STATE -> GCS"));

            // print state as hex to serial
            // auto *state_bytes = (uint8_t *)&state;
            // for (size_t i = 0; i < 79; i++)
            // {

            //     // print leading zero if needed
            //     if (state_bytes[i] < 0x10)
            //     {
            //         Serial.print("0");
            //     }
            //     Serial.print(state_bytes[i], HEX);
            // }
            // Serial.println();
        }
    }

    // TODO save state to SD card
}