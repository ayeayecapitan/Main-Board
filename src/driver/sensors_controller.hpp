#pragma once

#include <Adafruit_INA260.h>
#include <Adafruit_MAX31865.h>

#include "hardware/ds18b20.hpp"
#include "hardware/honeywell.hpp"

#include "shared/data.hpp"
#include "shared/system_state.hpp"

class SensorsController
{
    DS18B20Sensors _ds18b20_sensors;
    HoneywellSensor _honeywell_sensor;
    Adafruit_INA260 _ina260;
    
    Adafruit_MAX31865 _pt100_1 {driver_board::pt100::spi::PT100_1};
    Adafruit_MAX31865 _pt100_2 {driver_board::pt100::spi::PT100_2};

    public:
        SensorsController() = default;

        void init(){
            _ds18b20_sensors.init();
            _honeywell_sensor.init();
            _ina260.begin(driver_board::ina260::i2c::ADDRESS);
            _pt100_1.begin();
            _pt100_2.begin();
        }

        void update()
        {
            _ds18b20_sensors.update();
            _honeywell_sensor.update();
        }

        SystemState::SensorsData read()
        {
            SystemState::SensorsData data;
            // Temperature
            data.temperatures_c[temperature::index::DS18B20_1_OUTSIDE] = _ds18b20_sensors.readTemperatureC(driver_board::ds18b20::one_wire::index::DS18B20_1_OUTSIDE);
            data.temperatures_c[temperature::index::DS18B20_2_OUTSIDE] = _ds18b20_sensors.readTemperatureC(driver_board::ds18b20::one_wire::index::DS18B20_2_OUTSIDE);
            data.temperatures_c[temperature::index::DS18B20_3_OUTSIDE] = _ds18b20_sensors.readTemperatureC(driver_board::ds18b20::one_wire::index::DS18B20_3_OUTSIDE);
            data.temperatures_c[temperature::index::DS18B20_4_OUTSIDE] = _ds18b20_sensors.readTemperatureC(driver_board::ds18b20::one_wire::index::DS18B20_4_OUTSIDE);
            data.temperatures_c[temperature::index::DS18B20_5_INSIDE] = _ds18b20_sensors.readTemperatureC(driver_board::ds18b20::one_wire::index::DS18B20_5_INSIDE);
            data.temperatures_c[temperature::index::PT100_1] = pt100ToCelsius(_pt100_1.readRTD());
            data.temperatures_c[temperature::index::PT100_2] = pt100ToCelsius(_pt100_2.readRTD());
            data.temperatures_c[temperature::index::HONEYWELL] = _honeywell_sensor.readTemperatureC();

            // Pressure
            data.pressure_pa = _honeywell_sensor.readPressurePa();

            // Power supply
            data.voltage = _ina260.readBusVoltage();
            data.current = _ina260.readCurrent();
            data.power = _ina260.readPower();

            return data;
        }

    private:
        float pt100ToCelsius(uint16_t raw)
        {
            return (raw / driver_board::pt100::MAX_RAW_VALUE) * driver_board::pt100::REFERENCE_RESISTANCE;
        }

};

