#include <Arduino.h>
#include "shared/data.hpp"
#include "shared/system_state.hpp"

#include "sensors_controller.hpp"
#include "probes_controller.hpp"

#include "communication/main_board_interface.hpp"

MainBoardInterface main_board_interface(driver_board::I2C_ADDRESS);

SensorsController sensors;

SpeProbe SpeProbe[probe::COUNT] {
    {probe::index::PROBE_1},
    {probe::index::PROBE_2},
    {probe::index::PROBE_3},
    {probe::index::PROBE_4}
};

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

    // TEST

    String command;
    command = Serial.readString();
    bool ok = false;

    int probe_number = -1;

    while (!ok) {
        Serial.print("Type start [probe_number 1-4] to open valves and then start pump");
        Serial.println("Type stop [probe_number 1-4] to stop pump and then close valves");
        if (command.startsWith("start") && command.length() == 7)
        {
            probe_number = command[6] - '0' - 1;
            if (probe_number >= 0 && probe_number < probe::COUNT)
            {
                ok = true;
                Serial.println("Starting probe " + String(probe_number));
                SpeProbe[probe_number].start();
            }
                
        }
        else if (command.startsWith("stop") && command.length() == 6)
        {
            probe_number = command[5] - '0' - 1;
            if (probe_number >= 0 && probe_number < probe::COUNT)
            {
                ok = true;
                Serial.println("Stopping probe " + String(probe_number));
                SpeProbe[probe_number].stop();
            }
        }
        if (!ok)
            Serial.println("Invalid command");
    }




    // read from Serial 


    main_board_interface.resume();

    Serial.println(state);

    delay(3000);
}