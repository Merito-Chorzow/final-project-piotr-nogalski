#pragma once
#include <vector>
#include <cstdio>
#include "../drivers/TempSensor.h"
#include "../drivers/FanMotor.h"
#include "../drivers/Buzzer.h"

struct CurvePoint {
    float temp;
    int duty;
};

class FanController {
private:
    // HAL
    TempSensor& sensor;
    FanMotor& motor;
    Buzzer& buzzer;
    
    // Stan wewnętrzny
    float currentTemp = 0.0f;
    int targetDuty = 0;
    int currentDuty = 0;
    bool faultState = false;

    // Krzywa chłodzenia (LUT)
    std::vector<CurvePoint> curve = {
        {20.0f, 0},   // < 20C -> Stop
        {30.0f, 100}, // 30C -> 40% mocy
        {50.0f, 200}, // 50C -> 80% mocy
        {70.0f, 255}  // > 70C -> MAX
    };

    // Interpolacja liniowa
    int calculateDuty(float t) {
        if (t < curve.front().temp) return curve.front().duty;
        if (t > curve.back().temp) return curve.back().duty;

        for (size_t i = 0; i < curve.size() - 1; i++) {
            if (t >= curve[i].temp && t <= curve[i+1].temp) {
                float slope = (float)(curve[i+1].duty - curve[i].duty) / 
                              (curve[i+1].temp - curve[i].temp);
                return curve[i].duty + (int)(slope * (t - curve[i].temp));
            }
        }
        return 0;
    }

public:
    FanController(TempSensor& s, FanMotor& m, Buzzer& b) 
        : sensor(s), motor(m), buzzer(b) {}

    void init() {
        sensor.init();
        motor.init();
        buzzer.init();
    }

    void update() {
        float t = sensor.getTemperature();

        // Kontrola bezpieczeństwa
        if (t < -100.0f) { 
            faultState = true;
            targetDuty = 255;       // Maksymalne chłodzenie
            motor.setFaultLed(true);
            buzzer.setAlarm(true);
        } else {
            // Normalne działanie
            faultState = false;
            currentTemp = t;
            targetDuty = calculateDuty(t);
            motor.setFaultLed(false);
            buzzer.setAlarm(false);
        }

        // Kontrola stanu Normalny/Awaria
        if (currentDuty < targetDuty) {
            currentDuty += RAMP_STEP;
            if (currentDuty > targetDuty) currentDuty = targetDuty;
        } else if (currentDuty > targetDuty) {
            currentDuty -= RAMP_STEP;
            if (currentDuty < targetDuty) currentDuty = targetDuty;
        }


        motor.setDuty(currentDuty);
    }

    // Statystyki
    void getStatus(char* buffer) {
        sprintf(buffer, "STATUS: Temp=%.1f C, PWM=%d/255, FAULT=%d", 
                currentTemp, currentDuty, faultState);
    }

    // Protokół
    void setCurveOffset(int offset) {
        for(auto& p : curve) {
            p.duty += offset;
            if(p.duty > 255) p.duty = 255;
            if(p.duty < 0) p.duty = 0;
        }
    }
};