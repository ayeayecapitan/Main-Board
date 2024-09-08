#pragma once
#include "shared/data.hpp"
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

class DS18B20Sensors {
    OneWire _ds18b20_one_wire {driver_board::ds18b20::one_wire::PIN};
    DallasTemperature _ds18b20_sensors {&_ds18b20_one_wire};
    DeviceAddress _ds18b20_addr[driver_board::ds18b20::COUNT] = {0};
    public:

        DS18B20Sensors() = default;

        void init(){
            _ds18b20_sensors.begin();
            Serial.print("Number of DS18B20 sensors detected: ");
            Serial.print(_ds18b20_sensors.getDeviceCount(), DEC);
            Serial.println(" devices.");

            for(uint8_t index = 0; index < driver_board::ds18b20::COUNT; index++)
            {
                DeviceAddress& address = _ds18b20_addr[index];

                if(!indexValid(index))
                {
                    Serial.print("[DS18B20Sensors::init] Index ");
                    Serial.print(index, DEC);
                    Serial.println(" out of range");
                    continue;
                }

                if(!_ds18b20_sensors.getAddress(address, index))
                {
                    // Clear the address if not found (for some reason the library writes the previous address)
                    clearAddress(address);
                    Serial.print("WARNING: Unable to find address for DS18B20 sensor with index ");
                    Serial.println(index, DEC);
                    continue;
                }

                // Set resolution if found
                _ds18b20_sensors.setResolution(address, driver_board::ds18b20::PRECISION);

                Serial.println("Found address of DS18B20 sensor with index ");
                Serial.println(index, DEC);
            }
        }

        void update()
        {
            _ds18b20_sensors.requestTemperatures();
        }

        float readTemperatureC(uint8_t one_wire_index)
        {
            if(!indexValid(one_wire_index))
            {
                Serial.println("[DS18B20Sensors::read] Index out of range");
                return NAN;
            }

            DeviceAddress &address = _ds18b20_addr[one_wire_index];
            return _ds18b20_sensors.getTempC(address);
        }


    private:
        static void clearAddress(DeviceAddress &address)
        {
            for(auto & byte : address)
                byte = 0;
        }

        static bool indexValid(uint8_t index)
        {
            return index < driver_board::ds18b20::COUNT;
        }
};
