#pragma once

#include <Adafruit_INA260.h>
#include <Adafruit_MAX31865.h>

#include <TinyGPS++.h>
#include "hardware/ds18b20.hpp"
#include "hardware/honeywell.hpp"

#include "shared/data.hpp"
#include "shared/system_state.hpp"
#include "shared/debug.hpp"

class SensorsController
{
    DS18B20Sensors _ds18b20_sensors;
    HoneywellSensor _honeywell_sensor;
    Adafruit_INA260 _ina260;
    
    Adafruit_MAX31865 _pt100_1 {driver_board::pt100::spi::PT100_1};
    Adafruit_MAX31865 _pt100_2 {driver_board::pt100::spi::PT100_2};

    TinyGPSPlus _gps;
    SystemState::SensorsData::Gps _gps_data;
    uint32_t _last_gps_time = 0;
    uint32_t _last_gps_read_board_time = 0;

    public:
        SensorsController() = default;

        void init(){
            _ds18b20_sensors.init();
            _honeywell_sensor.init();
            _ina260.begin(driver_board::ina260::i2c::ADDRESS);
            _ina260.setAveragingCount(INA260_COUNT_16);
            _ina260.setMode(INA260_MODE_CONTINUOUS);
            _ina260.setVoltageConversionTime(INA260_TIME_140_us);
            _ina260.setCurrentConversionTime(INA260_TIME_140_us);

            _pt100_1.begin();
            _pt100_2.begin();

            // GPS setup
            constexpr uint32_t GPS_BAUD = 115200;
            Serial1.begin(GPS_BAUD);
        }

        SystemState::SensorsData read()
        {
            update();

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
            data.power_supply.voltage = _ina260.readBusVoltage();
            data.power_supply.current = _ina260.readCurrent();
            data.power_supply.power = _ina260.readPower();

            // GPS
            data.gps = _gps_data;

            return data;
        }

    private:
        float pt100ToCelsius(uint16_t raw)
        {
            return (raw / driver_board::pt100::MAX_RAW_VALUE) * driver_board::pt100::REFERENCE_RESISTANCE;
        }

        void update()
        {
            _ds18b20_sensors.update();
            _honeywell_sensor.update();
            updateGPS();
        }

        void updateGPS()
        {
            auto gps_read_start = millis();
            long gps_read_board_time;
            while (Serial1.available() > 0)
            {
                if(millis() - gps_read_start > 10)
                {
                    DEBUG_PRINTLN("GPS READING TIMEOUT");
                    break;
                }

                auto read_finished = _gps.encode(Serial1.read());
                if(read_finished)
                {
                    gps_read_board_time = millis();
                    break;
                }
            }


            if(_gps.location.isValid())
            {
                _gps_data.latitude = _gps.location.lat();
                _gps_data.longitude = _gps.location.lng();
            }

            if(_gps.altitude.isValid())
            {
                _gps_data.altitude = _gps.altitude.meters();
            }

            if(_gps.satellites.isValid())
            {
                _gps_data.satellites = _gps.satellites.value();
            }

            if(_gps.time.isValid())
            {
                _last_gps_time = _gps.time.value();
                _last_gps_read_board_time = gps_read_board_time;
                // DEBUG_PRINTLN("GPS TIME: " + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
            }
        }
};

