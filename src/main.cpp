#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
// #include <ICMPPing.h>


//TODO:
//1. Change from global
//


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // NOLINT
byte ip[] = { 172, 16, 18, 181 };  
IPAddress pingAddr(74,125,26,147); 


void setup() {
  EthernetClass::begin(mac, ip);

  const int BAUD_RATE = 9600;
  Serial.begin(BAUD_RATE);
}

void loop() {

}
