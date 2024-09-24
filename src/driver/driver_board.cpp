#include <Arduino.h>
#include "shared/data.hpp"
#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"

#include "sensors_controller.hpp"

#include "spe_probe.hpp"
#include "chemical_probe.hpp"

#include "main_board_interface.hpp"

#include <Wire.h>

MainBoardInterface main_board_interface(driver_board::I2C_ADDRESS);

SensorsController sensors;

SpeProbe spe_probes[probe::COUNT] {
    {probe::index::PROBE_1},
    {probe::index::PROBE_2},
    {probe::index::PROBE_3},
    {probe::index::PROBE_4}
};

SystemState state;
GroundCommand command;

void setup()
{
    Serial.begin(driver_board::SERIAL_BAUD_RATE);
    delay(1500); // Workaround for the serial monitor permissions issue - see after_upload in env.py

    sensors.init();
    main_board_interface.init();
}

SpeProbe * getActiveProbe()
{
    for (auto &probe : spe_probes)
    {
        if (probe.isActive())
            return &probe;
    }
    return nullptr;
}

SpeProbe * getNextProbeToStart(GroundCommand * command)
{
    for (auto &probe : spe_probes)
    {
        if (command->spe_probes_desired_states[probe.index()] == probe::State::ON)
            return &probe;
    }
    return nullptr;
}

void loop()
{
    // Update timestamp
    state.timestamp_us = micros();

    // Update sensors
    state.sensors = sensors.read();

    // Update valve states
    for (auto &probe : spe_probes)
        probe.getValveStates(state.devices.valve_state);

    // Update pump states
    state.devices.spe_pump_state = SpeProbe::pumpState();
    state.devices.chemical_pump_state = ChemicalProbe::pumpState();

    // Disable I2C interrupts
    main_board_interface.pause();

    // Push new state to main board interface
    main_board_interface.setState(state);
   
    // Check if new command is available
    if(main_board_interface.commandAvailable(command.timestamp_us))
    {
        command = main_board_interface.latestCommand();
        Serial.print("RECEIVED\t");
        Serial.println(command);
    }

    // Check if any probe is ON
    SpeProbe * active_probe = getActiveProbe();

    // If a probe is active, then we can only wait for close command
    if (active_probe != nullptr)
    {
        // Check if the command is to close the active probe
        auto active_probe_desired_state = command.spe_probes_desired_states[active_probe->index()];
        if (active_probe_desired_state == probe::State::OFF)
        {
            Serial.println("STOPPING ACTIVE PROBE " + String(active_probe->index() + 1));
            active_probe->stop();
        }
    }
    // If no probe is active, then we can start a new probe
    else
    {
        // Check if any probe is requested to be turned ON
        SpeProbe * next_probe_to_start = getNextProbeToStart(&command);
        if (next_probe_to_start != nullptr)
        {
            Serial.println("STARTING PROBE " + String(next_probe_to_start->index() + 1));
            next_probe_to_start->start();
        }
    }

    // Enable I2C interrupts
    main_board_interface.resume();
    delay(100);
}