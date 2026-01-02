#pragma once


constexpr int PIN_TEMP_SENSOR = 34; // Potencjometr 
constexpr int PIN_PWM_FAN = 2;  // Niebieska LED 
constexpr int PIN_FAULT_LED = 4;  // Czerwona LED 
constexpr int PIN_BUZZER = 5;  // Buzzer 


constexpr int PWM_FREQ = 25000;
constexpr int PWM_RES = 8;  


constexpr int TEMP_UPDATE_MS  = 100;    
constexpr int RAMP_STEP = 5;      
constexpr unsigned long SERIAL_BAUD = 115200;


constexpr int SENSOR_MIN_VALID = 50;    // zwarcie
constexpr int SENSOR_MAX_VALID = 4050;  // rozwarcie