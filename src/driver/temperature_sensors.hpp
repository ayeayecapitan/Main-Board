#pragma once
#include "shared/data.hpp"
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

class TemperatureSensors {
    OneWire _ds18b20_one_wire {temperature::driver_board::one_wire::pin::DS18B20_ALL};
    DallasTemperature _ds18b20_sensors {&_ds18b20_one_wire};
    DeviceAddress _ds18b20_addr[temperature::DS18B20_COUNT] = {0};
    public:

        static constexpr uint8_t DS18B20_PRECISION = 12;

        TemperatureSensors() = default;

        void init(){
            _ds18b20_sensors.begin();
            Serial.print("Number of DS18B20 sensors detected: ");
            Serial.print(_ds18b20_sensors.getDeviceCount(), DEC);
            Serial.println(" devices.");

            for(uint8_t i = 0; i < temperature::DS18B20_COUNT; i++)
            {
                if(getDs18b20Address(i))
                    _ds18b20_sensors.setResolution(_ds18b20_addr[i], DS18B20_PRECISION);
            }
        }

        void read(float temperature_out[temperature::COUNT])
        {
            _ds18b20_sensors.requestTemperatures();

            temperature_out[temperature::state_index::DS18B20_1_OUTSIDE] = getDs18b20Temperature(temperature::driver_board::one_wire::index::DS18B20_1_OUTSIDE);
            temperature_out[temperature::state_index::DS18B20_2_OUTSIDE] = getDs18b20Temperature(temperature::driver_board::one_wire::index::DS18B20_2_OUTSIDE);
            temperature_out[temperature::state_index::DS18B20_3_OUTSIDE] = getDs18b20Temperature(temperature::driver_board::one_wire::index::DS18B20_3_OUTSIDE);
            temperature_out[temperature::state_index::DS18B20_4_OUTSIDE] = getDs18b20Temperature(temperature::driver_board::one_wire::index::DS18B20_4_OUTSIDE);
            temperature_out[temperature::state_index::DS18B20_5_INSIDE] = getDs18b20Temperature(temperature::driver_board::one_wire::index::DS18B20_5_INSIDE);

            // TODO Add Honeywell and PT100 sensors
        }


    private:
        bool getDs18b20Address(uint8_t index)
        {
            if(index > temperature::DS18B20_COUNT)
            {
                Serial.println("get_ds18b20_address: Index out of range");
                return false;
            }

            if(!_ds18b20_sensors.getAddress(_ds18b20_addr[index], index))
            {
                // Clear the address if not found (for some reason the library writes the previous address)
                for(uint8_t i = 0; i < sizeof(DeviceAddress); i++)
                    _ds18b20_addr[index][i] = 0;

                Serial.print("WARNING: Unable to find address for DS18B20 sensor with index ");
                Serial.println(index, DEC);
                return false;
            }

            Serial.println("Found address of DS18B20 sensor with index ");
            Serial.println(index, DEC);
            return true;
        }

        float getDs18b20Temperature(uint8_t index)
        {
            if(index > temperature::DS18B20_COUNT)
            {
                Serial.println("get_ds18b20_temperature: Index out of range");
                return NAN;
            }

            return _ds18b20_sensors.getTempC(_ds18b20_addr[index]);
        }
};
