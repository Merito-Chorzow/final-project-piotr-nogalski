#pragma once
#include <Arduino.h>
#include "../include/Config.h"

class FanMotor {
public:
    void init() {
        // Konfiguracja PWM
        ledcAttach(PIN_PWM_FAN, PWM_FREQ, PWM_RES);
        ledcWrite(PIN_PWM_FAN, 0);
        
        pinMode(PIN_FAULT_LED, OUTPUT);
        digitalWrite(PIN_FAULT_LED, LOW);
    }

    void setDuty(int duty) {
        // Zabezpieczenie zakresu
        if (duty > 255) duty = 255;
        if (duty < 0) duty = 0;
        ledcWrite(PIN_PWM_FAN, duty);
    }

    void setFaultLed(bool state) {
        digitalWrite(PIN_FAULT_LED, state ? HIGH : LOW);
    }
};