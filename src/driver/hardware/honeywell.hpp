#include "shared/data.hpp"
#include <Wire.h>
#include <Arduino.h>
#include "shared/debug.hpp"


class HoneywellSensor
{
    public: 
        float pressure_pa = NAN;
        float temperature_c = NAN;

    void init()
    {
        
    }

    void update()
    {
        read(driver_board::honeywell::i2c::ADDRESS);
    }

    float readPressurePa()
    {
        return pressure_pa;
    }

    float readTemperatureC()
    {
        return temperature_c;
    }

    void read(const uint8_t slave_addr)
    {
        constexpr size_t RAW_DATA_SIZE = 4;
        constexpr int TIMEOUT_MS = 4;

        uint8_t raw_bytes[RAW_DATA_SIZE] = { 0 };

        Wire.setTimeout(TIMEOUT_MS);
        Wire.requestFrom(slave_addr, (uint8_t) RAW_DATA_SIZE);

        auto num_read = Wire.readBytes(raw_bytes, RAW_DATA_SIZE);
        if (num_read != RAW_DATA_SIZE)
        {
            DEBUG_PRINTLN("Error reading from Honeywell sensor");
        }

        struct {
            uint8_t status = 0;             // 2 bit
            uint16_t bridge_data = 0;       // 14 bit
            uint16_t temperature_data = 0;  // 11 bit
        } raw;

        constexpr uint8_t STATUS_OK = 0;
        constexpr uint8_t STATUS_COMMAND = 1;
        constexpr uint8_t STATUS_STALE = 2;
        constexpr uint8_t STATUS_DIAGNOSTIC_FAULT = 3;
        
        raw.status = (raw_bytes[0] & 0xc0) >> 6;  // first 2 bits from first byte
        raw.bridge_data = (((uint16_t) (raw_bytes[0] & 0x3f)) << 8) + raw_bytes[1];
        raw.temperature_data = ((raw_bytes[2] << 8) + raw_bytes[3])>>5;

        if(raw.status != STATUS_OK)
        {
            DEBUG_PRINTLN("Honewell error status: ");
            switch (raw.status)
            {
                case STATUS_COMMAND:
                    DEBUG_PRINTLN("Command");
                    break;
                case STATUS_STALE:
                    DEBUG_PRINTLN("Stale");
                    break;
                case STATUS_DIAGNOSTIC_FAULT:
                    DEBUG_PRINTLN("Diagnostic fault");
                    break;
                default:
                    DEBUG_PRINTLN("Unknown");
                    break;
            }
            pressure_pa = NAN;
            temperature_c = NAN;
            return;
        }

        // TODO verify the conversion
        constexpr int OUTPUT_MIN = 0x666;        // 10%
        constexpr int OUTPUT_MAX = 0x399A;       // 90% of 2^14 - 1
        constexpr float PRESSURE_MIN = 0.0;      // min is 0 for sensors that give absolute values
        constexpr float PRESSURE_MAX = 206842.7/2; // 30psi/2 (and we want results in pascals)

        // it measures   100730 when it should be 101200
        // TODO: verify this
        constexpr float PRESSURE_MAX_CORRECTED = PRESSURE_MAX * (101200.0 / 100730.0);

        pressure_pa = 1.0 * ((int) raw.bridge_data - OUTPUT_MIN) * (PRESSURE_MAX_CORRECTED - PRESSURE_MIN) / (OUTPUT_MAX - OUTPUT_MIN) + PRESSURE_MIN;
        temperature_c = ((int)raw.temperature_data *200.0/2047) - 50;
    }



};