#include <Arduino.h>
#include <Ethernet.h>

#include "pins.hpp"
#include "ethernet_communication.hpp"
#include "pins_arduino.h"




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

void setup() {
    delay(1000); // wait for the serial monitor to open

    Serial.begin(9600);
    EthernetCommunication.init();

    //digitalWrite(1, 2);
}

unsigned long lastMicros = 0;

void loop() {
    auto now = micros();
    auto delta = now - lastMicros;
    lastMicros = now;

    Serial.print("Loop time: ");
    Serial.println(delta);

    EthernetCommunication.update();
    
}