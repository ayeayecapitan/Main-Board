#pragma once
#include "pins.hpp"

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

#include "network_config.hpp"

#include "shared/ground_command.hpp"
#include "shared/system_state.hpp"

class GroundStationInterface
{
    bool _initialized = false;

    EthernetUDP _gcs_udp;

    bool _command_available = false;
    GroundCommand _command;

public:
    // Delete copy and move constructors
    GroundStationInterface(const GroundStationInterface &) = delete;
    GroundStationInterface(GroundStationInterface &&) = delete;
    GroundStationInterface &operator=(const GroundStationInterface &) = delete;
    GroundStationInterface &operator=(GroundStationInterface &&) = delete;

    // One way to create the object
    GroundStationInterface() = default;

    bool init()
    {
        {
            using namespace network::main_board;
            uint8_t mac[6] = {MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]}; // NOLINT mac needs to be unsigned char * - not const
            EthernetClass::init(pinout::digital::ETHERNET);
            EthernetClass::begin(mac, network::main_board::IP);
            DEBUG_PRINT(F("Ethernet communication initialized with IP: "));
            IP.printTo(Serial);
            DEBUG_PRINTLN();
        }

        if (EthernetClass::hardwareStatus() == EthernetNoHardware)
        {
            DEBUG_PRINTLN(F("Ethernet shield was not found. Can't run without hardware."));
            return false;
        }
        if (EthernetClass::linkStatus() == LinkOFF)
        {
            DEBUG_PRINTLN(F("Ethernet cable is not connected."));
            return false;
        }

        if(_gcs_udp.begin(network::main_board::UDP_PORT) != 1)
        {
            DEBUG_PRINTLN(F("Failed to start UDP socket."));
            return false;
        }

        DEBUG_PRINT(F("UDP socket started on port "));
        DEBUG_PRINTLN(network::main_board::UDP_PORT);

        _initialized = true;
        return _initialized;
    }

    void processIncomingData()
    {
        if (!_initialized)
        {
            DEBUG_PRINTLN(F("Ethernet communication not initialized."));
            return;
        }

        // Check if there is a packet available
        int packet_size = _gcs_udp.parsePacket();
        if(packet_size == 0)
            return;

        if(packet_size < 0)
        {
            DEBUG_PRINTLN(F("Error reading packet."));
            return;
        }

        if (packet_size != sizeof(_command))
        {
            _gcs_udp.flush();
            DEBUG_PRINTLN(F("Packet size does not match GroundCommand size. Dropping."));
            return;
        }

        // After checks packet is available and with size 1 to sizeof(_receiveBuffer)
        _gcs_udp.read((uint8_t *)&_command, packet_size);
        _command_available = true;

   
    }

    bool commandAvailable(uint64_t last_stamp) const
    {
        return _command_available && _command.timestamp_us > last_stamp;
    }

    const GroundCommand& latestCommand()
    {
        return _command;
    }

    void sendState(const SystemState &state)
    {
        if (!_initialized)
        {
            DEBUG_PRINTLN(F("Ethernet communication not initialized."));
            return;
        }

        _gcs_udp.beginPacket(network::ground_station::IP, network::ground_station::UDP_PORT);
        _gcs_udp.write((uint8_t *)&state, sizeof(state));
        if (_gcs_udp.endPacket() != 1)
        {
            DEBUG_PRINTLN(F("Failed to send state to GCS."));
        }
    }

};
