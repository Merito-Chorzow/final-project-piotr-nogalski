#include <Arduino.h>
#include "../include/Config.h"
#include "../drivers/TempSensor.h"
#include "../drivers/FanMotor.h"
#include "../drivers/Buzzer.h"
#include "../app/FanController.h"

// --- OBIEKTY GLOBALNE ---
TempSensor sens;
FanMotor mot;
Buzzer buzz;
// Wstrzyknięcie sterowników do kontrolera
FanController fanCtrl(sens, mot, buzz);

unsigned long lastTick = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("--- SYSTEM START: Smart Fan Controller ---");
    Serial.println("--- CLI READY: commands: STAT, OFFSET <val> ---");
    
    fanCtrl.init();
}

void handleCli() {
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        
        if (cmd == "STAT") {
            char buf[64];
            fanCtrl.getStatus(buf);
            Serial.println(buf);
        } 
        else if (cmd.startsWith("OFFSET ")) {
            int val = cmd.substring(7).toInt();
            fanCtrl.setCurveOffset(val);
            Serial.printf("ACK: Curve offset applied: %d\n", val);
        }
        else {
            Serial.println("ERR: Unknown command");
        }
    }
}

void loop() {
    // Non-blocking scheduler
    unsigned long now = millis();
    if (now - lastTick >= TEMP_UPDATE_MS) {
        lastTick = now;
        fanCtrl.update();
    }
    
    // Obsługa komunikacji w czasie wolnym procesora
    handleCli();
}