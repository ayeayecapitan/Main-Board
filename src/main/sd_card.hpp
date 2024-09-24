#include <Arduino.h>   
#include <SD.h> 

class SDCard
{
    public:
        SDCard() = default;
        ~SDCard() = default;

        // NO COPY AND MOVE
        SDCard(const SDCard&) = delete;
        SDCard& operator=(const SDCard&) = delete;
        SDCard(SDCard&&) = delete;
        SDCard& operator=(SDCard&&) = delete;

        void init()
        {
            if (!SD.begin(7))
            {
                Serial.println("SD card initialization failed.");
                return;
            }
            Serial.println("SD card initialized.");
        }

        void write(const char* filename, const char* data)
        {
            File file = SD.open(filename, FILE_WRITE);
            if (!file)
            {
                Serial.println("Failed to open file for writing.");
                return;
            }

            file.println(data);
            file.close();
        }

        void read(const char* filename)
        {
            File file = SD.open(filename);
            if (!file)
            {
                Serial.println("Failed to open file for reading.");
                return;
            }

            while (file.available())
            {
                Serial.write(file.read());
            }
            file.close();
        }
};