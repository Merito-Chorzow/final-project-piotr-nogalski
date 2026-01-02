#include <Arduino.h>
#include "Config.h"


float temperatura = 0;
int pwm_cel = 0;
int pwm_obecny = 0;
bool blad_sensora = false;

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("INICJALIZACJA SYSTEMU (Wersja TESTOWA v0.1)...");

  pinMode(PIN_TEMP_SENSOR, INPUT);
  pinMode(PIN_FAULT_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  ledcAttach(PIN_PWM_FAN, PWM_FREQ, PWM_RES);
  
  Serial.println("Setup gotowy.");
}

void loop() {
  int surowe_adc = analogRead(PIN_TEMP_SENSOR);
  
  if (surowe_adc < SENSOR_MIN_VALID || surowe_adc > SENSOR_MAX_VALID) {
    blad_sensora = true;
  } else {
    blad_sensora = false;

    temperatura = (surowe_adc / 4095.0f) * 100.0f;
  }

  if (blad_sensora) {
    pwm_cel = 255; // Tryb awaryjny
    digitalWrite(PIN_FAULT_LED, HIGH);
    tone(PIN_BUZZER, 2000); 
  } else {
    digitalWrite(PIN_FAULT_LED, LOW);
    noTone(PIN_BUZZER);

    if (temperatura < 25) pwm_cel = 0;
    else if (temperatura < 40) pwm_cel = 100;
    else if (temperatura < 60) pwm_cel = 200;
    else pwm_cel = 255;
  }

  if (pwm_obecny < pwm_cel) {
    pwm_obecny += RAMP_STEP;
  } else if (pwm_obecny > pwm_cel) {
    pwm_obecny -= RAMP_STEP;
  }

  ledcWrite(PIN_PWM_FAN, pwm_obecny);

  Serial.print("Temp: "); Serial.print(temperatura);
  Serial.print(" | PWM: "); Serial.print(pwm_obecny);
  if (blad_sensora) Serial.print(" !!! BLAD !!!");
  Serial.println();

  delay(100); 
}