#include <Arduino.h>
#include "Config.h"

unsigned long lastUpdate = 0;
float currentTemp = 0;
int targetDuty = 0;
int actualDuty = 0;

// Funkcja pomocnicza do odczytu temperatury
float readTemp() {
    int raw = analogRead(PIN_TEMP_SENSOR);
    // Prosta weryfikacja błędu
    if (raw < SENSOR_MIN_VALID || raw > SENSOR_MAX_VALID) return -1.0;
    return (raw / 4095.0f) * 100.0f;
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    ledcAttach(PIN_PWM_FAN, PWM_FREQ, PWM_RES);
    pinMode(PIN_FAULT_LED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    Serial.println("System v0.2: Non-blocking with Ramp.");
}

void loop() {
    unsigned long now = millis();

    // Wykonuj logikę co 100ms (zamiast delay)
    if (now - lastUpdate >= TEMP_UPDATE_MS) {
        lastUpdate = now;

        float t = readTemp();

        if (t < 0) {
            // Tryb awaryjny
            targetDuty = 255;
            digitalWrite(PIN_FAULT_LED, HIGH);
            tone(PIN_BUZZER, 1000);
        } else {
            currentTemp = t;
            digitalWrite(PIN_FAULT_LED, LOW);
            noTone(PIN_BUZZER);

            // Proste mapowanie
            if (currentTemp < 25) targetDuty = 0;
            else if (currentTemp < 45) targetDuty = 120;
            else targetDuty = 255;
        }

        // --- ALGORYTM RAMPY (Slew-Rate Limiter) ---
        // Zamiast actualDuty = targetDuty, zmieniamy wartość o mały krok
        if (actualDuty < targetDuty) {
            actualDuty += RAMP_STEP;
            if (actualDuty > targetDuty) actualDuty = targetDuty;
        } 
        else if (actualDuty > targetDuty) {
            actualDuty -= RAMP_STEP;
            if (actualDuty < targetDuty) actualDuty = targetDuty;
        }

        ledcWrite(PIN_PWM_FAN, actualDuty);

        // Debugowanie postępów rampy
        Serial.printf("T: %.1f | Cel: %d | Real: %d\n", currentTemp, targetDuty, actualDuty);
    }
 
    // "TODO: Implementacja CLI w następnym etapie"
}