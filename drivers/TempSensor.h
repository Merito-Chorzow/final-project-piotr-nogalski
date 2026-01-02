#pragma once
#include <Arduino.h>
#include "../include/Config.h"

class TempSensor {
public:
    void init() {
        pinMode(PIN_TEMP_SENSOR, INPUT);
    }

    // Zwraca surową wartość ADC lub -1 w przypadku błędu sprzętowego
    int readRaw() {
        int val = analogRead(PIN_TEMP_SENSOR);
        if (val < SENSOR_MIN_VALID || val > SENSOR_MAX_VALID) {
            return -1; // Błąd: Sensor poza zakresem
        }
        return val;
    }

    // Zmiana na stopnie Celsjusza
    float getTemperature() {
        int raw = readRaw();
        if (raw == -1) return -999.0f; // Kod błędu
        // 0..4095 = 0..100 stopni C
        return (raw / 4095.0f) * 100.0f;
    }
};