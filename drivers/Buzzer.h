#pragma once
#include <Arduino.h>
#include "../include/Config.h"

class Buzzer {
public:
    void init() {
        pinMode(PIN_BUZZER, OUTPUT);
        digitalWrite(PIN_BUZZER, LOW);
    }

    void setAlarm(bool state) {
        if (state) {
            tone(PIN_BUZZER, 2000);
        } else {
            noTone(PIN_BUZZER);
            digitalWrite(PIN_BUZZER, LOW);
        }
    }
};