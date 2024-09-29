#include <Arduino.h>
#include <Wire.h>
#include <avr/wdt.h>

#include "USBAPI.h"
#include "shared/data.hpp"
#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"

#include "main_board_interface.hpp"
#include "sensors_controller.hpp"
#include "spe_probe.hpp"
#include "chemical_probe.hpp"
#include "shared/debug.hpp"

#include <TinyGPS++.h>

MainBoardInterface main_board_interface;

SensorsController sensors;

SpeProbe spe_probes[probe::COUNT] {
    {probe::index::PROBE_1},
    {probe::index::PROBE_2},
    {probe::index::PROBE_3},
    {probe::index::PROBE_4}
};

SystemState state;
GroundCommand command;

void enableWatchdog()
{
    delay(5000);
    wdt_enable(WDTO_8S);
    wdt_reset();
}

void setup()
{
    // enableWatchdog(); TODO Enable watchdog
    Serial.begin(driver_board::SERIAL_BAUD_RATE);
    while (!Serial); // Wait for serial port to connect.
    delay(1500); // Workaround for the serial monitor permissions issue - see after_upload in env.py

    DEBUG_PRINTLN("STARTING DRIVER BOARD");

    sensors.init();
    main_board_interface.init();
    ChemicalProbe::init();
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
    //wdt_reset();

    // Update sensors, disable I2C interrupts because some sensors are read using I2C
    state.timestamp_us = millis() * 1000;
    // main_board_interface.pause();
    state.sensors = sensors.read();
    // main_board_interface.resume();

    // Update valve states
    for (auto &probe : spe_probes)
        probe.getValveStates(state.devices.valves_state);

    // Update pump states
    state.devices.spe_pump_state = SpeProbe::pumpState();
    state.devices.chemical_pump_state = ChemicalProbe::pumpState();


    // Disable I2C interrupts
    main_board_interface.pause();

    // Push new state to main board interface
    main_board_interface.setState(state);
    DEBUG_PRINTLN(state);
   
    // Check if new command is available
    if(main_board_interface.commandAvailable(command.timestamp_us))
    {
        command = main_board_interface.latestCommand();
        DEBUG_PRINT("RECEIVED\t");
        DEBUG_PRINTLN(command);
    }

    // If a probe is active, then we only wait for close command
    SpeProbe * active_probe = getActiveProbe();
    if (active_probe != nullptr)
    {
        // Check if the command is to close the active probe
        auto active_probe_desired_state = command.spe_probes_desired_states[active_probe->index()];
        if (active_probe_desired_state == probe::State::OFF)
        {
            DEBUG_PRINTLN("STOPPING ACTIVE PROBE " + String(active_probe->index() + 1));
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
            DEBUG_PRINTLN("STARTING PROBE " + String(next_probe_to_start->index() + 1));
            next_probe_to_start->start();
        }
    }

    // Handle chemical probe
    if (command.chemical_probe_desired_state == probe::State::ON && ChemicalProbe::pumpState() != pump::State::ON)
    {
        DEBUG_PRINTLN("STARTING CHEMICAL PROBE");
        ChemicalProbe::start();
    }

    if (command.chemical_probe_desired_state == probe::State::OFF && ChemicalProbe::pumpState() != pump::State::OFF)
    {
        DEBUG_PRINTLN("STOPPING CHEMICAL PROBE");
        ChemicalProbe::stop();
    }

    // Enable I2C interrupts
    main_board_interface.resume();

    delay(100);
}