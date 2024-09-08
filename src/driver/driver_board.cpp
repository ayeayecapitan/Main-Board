#include <Arduino.h>
#include "shared/data.hpp"
#include "shared/system_state.hpp"

#include "sensors_controller.hpp"

#include "communication/main_board_interface.hpp"

MainBoardInterface main_board_interface(driver_board::I2C_ADDRESS);

SensorsController sensors;


SystemState state;

void setup()
{
    delay(1000); // wait for the monitor serial port to be available
    Serial.begin(driver_board::SERIAL_BAUD_RATE);

    sensors.init();
    main_board_interface.init();
}

GcsCommand command;

void loop()
{
    if(main_board_interface.commandAvailable() && main_board_interface.latestCommand().timestamp_us > command.timestamp_us)
    {
        command = main_board_interface.latestCommand();

        Serial.println("RECEIVED COMMAND");
        Serial.println(command);
    }

    state.timestamp_us = micros();

    // Read sensors
    sensors.update();
    state.sensors = sensors.read();

    // Read GPS
    // state.gps = // TODO

    // Read supply
    // state.supply = // TODO

    // Read devices
    // state.devices = // TODOs

    // Disable I2C interrupts to handle valve motors safely
    main_board_interface.pause();

    // Handle probes

    main_board_interface.resume();

    Serial.println(state);

    delay(3000);
}