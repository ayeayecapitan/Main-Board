// #pragma once
// #include "shared/data.hpp"
// #include <Arduino.h>
// #include "SparkFun_u-blox_GNSS_Arduino_Library.h"
// #include <Wire.h>

// class GPS {
//     public:
//             float altitude = 0;
//             float latitude = 0;
//             float longitude = 0;
//             uint8_t satellites = 0;
        
//             SFE_UBLOX_GNSS myGNSS;

//         void init(){    
//             Wire.begin();
//             if(myGNSS.begin()==false)
//             {
//                 Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
//                 while(1);
//             }
//             else
//             {
//                 Serial.println(F("Ublox GPS detected at default I2C address."));
//             }
//         }
        
//         // //Update method that reads the GPS data and stores it in the class fields
//         // void update()
//         // {
//         //     if(gpsHelper.isConnected())
//         //     {
//         //         if(gpsHelper.getAltitude() != altitude)
//         //         {
//         //             altitude = gpsHelper.getAltitude();
//         //         }
//         //         if(gpsHelper.getLatitude() != latitude)
//         //         {
//         //             latitude = gpsHelper.getLatitude();
//         //         }
//         //         if(gpsHelper.getLongitude() != longitude)
//         //         {
//         //             longitude = gpsHelper.getLongitude();
//         //         }
//         //         if(gpsHelper.getSIV() != satellites)
//         //         {
//         //             satellites = gpsHelper.getSIV();
//         //         }
//         //     }
//         //     else {
//         //         Serial.println("GPS not connected");
//         //     }
//         // }

//         // float readAltitude()
//         // {
//         //     return altitude;
//         // }

//         // float readLatitude()
//         // {
//         //     return latitude;
//         // }

//         // float readLongitude()
//         // {
//         //     return longitude;
//         // }

//         // uint8_t readSatellites()
//         // {
//         //     return satellites;
//         // }

// };