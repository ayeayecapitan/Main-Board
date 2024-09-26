#pragma once
#include <Arduino.h>
#include "shared/data.hpp"
#include <pins.hpp>
#include <SD.h>

class SDController
{
    File _file;
    bool _is_open = false;

public:
    SDController() = default;

    ~SDController() = default;

    // NO COPY AND MOVE
    SDController(const SDController&) = delete;
    SDController& operator=(const SDController&) = delete;
    SDController(SDController&&) = delete;
    SDController& operator=(SDController&&) = delete;

    void init()
    {
        if(!SD.begin(pinout::digital::SD_CARD))
        {
            Serial.println(F("SD initialization failed"));
            return;
        }

        _file = SD.open("1.txt", FILE_WRITE);
        if(!_file)
        {
            Serial.println(F("Failed to open log file"));
            return;
        }

        _is_open = true;
    }

    void log(const String& message)
    {
        if(!_is_open)
        {
            Serial.println(F("Log file not open"));
            return;
        }

        _file.write((uint8_t*)message.c_str(), sizeof(message));
        _file.write('\n');
        _file.close();
    }
};



// #include <SD.h>
// #include <SPI.h>

// File _file;
// const int chipSelect = 10;  // Pin CS dla karty SD

// void setup() {
//   Serial.begin(9600);
  
//   // Inicjalizacja karty SD
//   if (!SD.begin(chipSelect)) {
//     Serial.println("SD card initialization failed!");
//     return;
//   }
  
//   // Znajdź plik z największym indeksem
//   int maxIndex = findMaxIndex();
//   Serial.print("Found maximum file index: ");
//   Serial.println(maxIndex);

//   // Otwórz plik z największym indeksem, aby do niego zapisać
//   String filename = String(maxIndex) + ".txt";
//   _file = SD.open(filename, FILE_WRITE);
  
//   if (_file) {
//     Serial.print("Writing to file: ");
//     Serial.println(filename);
    
//     // Zapisz dane do pliku
//     _file.println("Hello, World!");
    
//     // Zamknij plik po zapisie
//     _file.close();
//     Serial.println("Done writing to file.");
//   } else {
//     Serial.println("Failed to open file for writing.");
//   }

//   // Indeks do nowego pliku
//   int newIndex = maxIndex + 1;
//   Serial.print("New file should have index: ");
//   Serial.println(newIndex);
// }

// void loop() {
//   // Nic tu nie robimy
// }

// // Funkcja przeszukująca pliki na karcie SD i znajdująca największy indeks
// int findMaxIndex() {
//   int maxIndex = 0;

//   // Otwórz katalog główny na karcie SD
//   File root = SD.open("/");
//   File entry;

//   // Iteruj przez wszystkie pliki w katalogu
//   while (entry = root.openNextFile()) {
//     String fileName = entry.name();

//     // Sprawdź czy plik ma rozszerzenie ".txt"
//     if (fileName.endsWith(".txt")) {
//       // Wyciągnij część liczbową z nazwy pliku
//       String baseName = fileName.substring(0, fileName.length() - 4);  // Usuwa ".txt"
//       int fileIndex = baseName.toInt();  // Konwertuje na liczbę
      
//       // Jeśli udało się poprawnie przekonwertować i indeks jest większy niż dotychczasowy max
//       if (fileIndex > 0 && fileIndex > maxIndex) {
//         maxIndex = fileIndex;
//       }
//     }
//     entry.close();  // Zamknij plik, aby przejść do kolejnego
//   }

//   return maxIndex;  // Zwróć największy znaleziony indeks
// }
