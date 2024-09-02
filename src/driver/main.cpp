#include <Arduino.h>
#include "shared/data.hpp"
#include "shared/i2c_slave.hpp"

#include "pins_arduino.h"

I2CSlave slave(driver_board::I2C_ADDRESS);

//SystemState state;

void setup() {
    delay(1000); // wait for the monitor serial port to be available
    Serial.begin(115200);

    slave.init();
}


void loop() {
    // Nothing to do here, communication handled by interrupts
}