#pragma once
#include <Arduino.h>
#include "shared/data.hpp"
#include <EEPROM.h>


class EEPROMController{
  private:
    uint32_t _rebootCount;  

  public:
    EEPROMController() {
      _rebootCount = 0;
    }

    void begin() {
      EEPROM.get(main_board::EEEPRM_ADDRESS, _rebootCount); 
      _rebootCount++;
      EEPROM.put(main_board::EEEPRM_ADDRESS, _rebootCount);
    }

    uint32_t getRebootCount() {
      return _rebootCount;
    }
};