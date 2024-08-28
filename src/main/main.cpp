#include <Arduino.h>
#include <Ethernet.h>
#include "pins.h"
#include "I2CScanner.h"

#include "shared/i2c_master.hpp"



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





//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address
//byte ip[] = { 172, 16, 18, 181 };  // IP address


// I2CMaster master{0x42};

I2CScanner scanner;
void setup() {
    Serial.begin(9600);
    while (!Serial) {};
    scanner.Init(); 
    // const char *message = "Hello, Slave!";
    // master.sendMessage(message);
    // Serial.println("Message sent to slave.");
}

void loop() {
    scanner.Scan();
    delay(5000);
}