#pragma once
#include "USBAPI.h"
#include "pins.hpp"

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

#include "data.hpp"

#include "network_config.hpp"

class EthernetCommunication
{
    bool _initialized = false;

    EthernetUDP _gcs_udp;

    GcsCommand _command;
    MainBoardState _state;

public:
    // Delete copy and move constructors
    EthernetCommunication(const EthernetCommunication &) = delete;
    EthernetCommunication(EthernetCommunication &&) = delete;
    EthernetCommunication &operator=(const EthernetCommunication &) = delete;
    EthernetCommunication &operator=(EthernetCommunication &&) = delete;

    // One way to create the object
    EthernetCommunication() = default;

    bool init()
    {
        {
            using namespace network::main_board;
            uint8_t mac[6] = {MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]}; // NOLINT mac needs to be unsigned char * - not const
            EthernetClass::init(pinout::digital::ETHERNET);
            EthernetClass::begin(mac, network::main_board::IP);
            Serial.print("Ethernet communication initialized with IP: ");
            IP.printTo(Serial);
            Serial.println();
        }

        if (EthernetClass::hardwareStatus() == EthernetNoHardware)
        {
            Serial.println("Ethernet shield was not found. Can't run without hardware.");
            return false;
        }
        if (EthernetClass::linkStatus() == LinkOFF)
        {
            Serial.println("Ethernet cable is not connected.");
            return false;
        }

        if(_gcs_udp.begin(network::main_board::UDP_PORT) != 1)
        {
            Serial.println("Failed to start UDP socket.");
            return false;
        }

        Serial.print("UDP socket started on port ");
        Serial.println(network::main_board::UDP_PORT);

        _initialized = true;
        return _initialized;
    }

    void update()
    {
        if (!_initialized)
        {
            Serial.println("Ethernet communication not initialized.");
            return;
        }

        // Check if there is a packet available
        int packet_size = _gcs_udp.parsePacket();
        if(packet_size == 0)
            return;

        if(packet_size < 0)
        {
            Serial.println("Error reading packet.");
            return;
        }

        if (packet_size != sizeof(_command))
        {
            _gcs_udp.flush();
            Serial.println("Packet size does not match GcsCommand size. Dropping.");
            return;
        }

        // After checks packet is available and with size 1 to sizeof(_receiveBuffer)
        _gcs_udp.read((uint8_t *)&_command, packet_size);

        // Process the command
        // Serial.print("Received command: ");
        // Serial.print(_command.type);
        // Serial.print(" with value: ");
        // Serial.println(_command.param);


        // Send state to gcs
        _state.timestamp_ns = micros() * 1000; // Todo use RTC
        _state.altitude = 100;
        _state.latitude = 12;
        _state.longitude = 10;
        _state.satellite_numbers = 5;
        _state.temperature = 25;

        _gcs_udp.beginPacket(network::ground_station::IP, network::ground_station::UDP_PORT);
        _gcs_udp.write((uint8_t *)&_state, sizeof(_state));
        if(_gcs_udp.endPacket() != 1)
        {
            Serial.println("Failed to send state to GCS.");
        }
    }
};

EthernetCommunication EthernetCommunication;
