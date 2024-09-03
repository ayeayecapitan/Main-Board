#include <Arduino.h>
#include "shared/data.hpp"
#include "shared/i2c_slave.hpp"
#include "temperature_sensors.hpp"
#include "pins_arduino.h"


DriverToMainComm slave(driver_board::I2C_ADDRESS);

TemperatureSensors TemperatureSensors;
//SystemState state;

void setup() {
    delay(1000); // wait for the monitor serial port to be available
    Serial.begin(115200);

    TemperatureSensors.init();
   // slave.init();
}


void loop() {

    delay(1000);

    float temperature[temperature::COUNT] = {0};
    TemperatureSensors.read(temperature);

    for (int i = 0; i < temperature::COUNT; i++)
    {
        Serial.print("Temperature ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(temperature[i]);
    }

    // Nothing to do here, communication handled by interrupts
}