#include <Arduino.h>
#include "shared/i2c_slave.hpp"

#define SLAVE_ADDRESS 0x08  // This should match the master's target address

I2CSlave slave(SLAVE_ADDRESS);

void setup() {
    Serial.begin(9600);
    Serial.println("Slave ready to receive messages.");
}


void loop() {
    // Nothing to do here, communication handled by interrupts
}