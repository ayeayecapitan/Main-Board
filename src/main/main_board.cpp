#include <Arduino.h>
#include <Ethernet.h>

#include "shared/debug.hpp"

#include "driver_board_interface.hpp"
#include "ground_station_interface.hpp"

#include "shared/data.hpp"
#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"

#include "hardware/eeprom_controller.hpp"

#include <avr/wdt.h>

#include "hardware/sd_controller.hpp"

DriverBoardInterface driver_board_interface;
GroundStationInterface ground_station_interface;

GroundCommand command;
uint64_t last_successful_state_request = 0;

EEPROMController eeprom;
SDController _sd_controller;

void enableWatchdog()
{
    delay(5000);
    wdt_enable(WDTO_8S);
    wdt_reset();

}

void setup() {
    // enableWatchdog(); TODO Enable watchdog
    Serial.begin(main_board::SERIAL_BAUD_RATE);
    while (!Serial);

    delay(1500); // Workaround for the serial monitor permissions issue - see after_upload in env.py

    eeprom.begin();

    ground_station_interface.init();
    driver_board_interface.init();

    _sd_controller.init();
}

void loop() {
    //wdt_reset();
    // Process incoming UDP data from the ground station
    ground_station_interface.processIncomingData();

    // If new command is available forward it to the driver board
    if(ground_station_interface.commandAvailable(command.timestamp_us))
    {
        command = ground_station_interface.latestCommand();
        driver_board_interface.sendCommand(command);
        // DEBUG_PRINTLN(command);
        DEBUG_PRINTLN(F("GCS CMD -> DRIVER"));
    }
    

    // Do every STATE_REQUEST_INTERVAL_MS
    constexpr uint64_t STATE_REQUEST_INTERVAL_MS = 1000;
    if(millis() - last_successful_state_request >= STATE_REQUEST_INTERVAL_MS)
    {
        // Request the state from the driver board and forward it to the ground station
 
        SystemState state;
        if(driver_board_interface.requestState(state))
        {
            last_successful_state_request = millis();
            ground_station_interface.sendState(state);
            _sd_controller.log(state);
            DEBUG_PRINTLN(F("DRIVER STATE -> GCS"));
        }
    }
}