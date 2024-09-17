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

void loop() {
    GroundStationInterface.processIncomingData();

    if(GroundStationInterface.commandAvailable(command.timestamp_us))
    {
        command = GroundStationInterface.latestCommand();

        Serial.println("RECEIVED COMMAND FROM GROUND STATION");
        Serial.println(command);
        driver_board_interface.sendCommand(command);
        Serial.println("FORWARDED COMMAND TO DRIVER BOARD");
    }
}

//Main board pinout
// GPS - UART, I2C 0x42
// SD card - SPI D7
// LED - D13
// Ethernet - SPI D5
// DS18B20 - D9 OneWire
//RTC I2C bazowo 0x68

/*TODO
komunikacja z driver boardem (I2C lub OneWire)

odczyt temperatury z DS18B20 (OneWire D9)

odczyt danych z GPS (I2C)

odczyt danych z RTC (I2C)

obsługa karty SD (SPI D7)

Obsługa ethernetu (SPI D5)

*/ 
//TODO:
//1. Downlink
//2. Uplink
//4. Frame signatures
//5. Telecommands
//6. Files
//8. Errr codes