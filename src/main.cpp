#include <Arduino.h>
#include "../include/Config.h"
#include "../drivers/TempSensor.h"
#include "../drivers/FanMotor.h"
#include "../drivers/Buzzer.h"
#include "../app/FanController.h"


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

        //Stat
        if (cmd == "STAT") {
            char buf[64];
            fanCtrl.getStatus(buf);
            Serial.println(buf);
        } 
        
        //Help
        if (cmd == "HELP" || cmd == "?") {
            Serial.println("--- DOSTEPNE KOMENDY ---");
            Serial.println(" STAT         - Pokaz status systemu (JSON-like)");
            Serial.println(" OFFSET <val> - Dodaj stala wartosc do PWM (-255..255)");
            Serial.println(" HELP         - Pokaz te liste");
            Serial.println("------------------------");
        }

        //Offset
        else if (cmd.startsWith("OFFSET ")) {
            int val = cmd.substring(7).toInt();
            if (val >= -255 && val <= 255) {
                fanCtrl.setCurveOffset(val);
                Serial.printf("OK: Offset ustawiony na %d\n", val);
            } else {
                Serial.println("ERR: Wartosc poza zakresem (-255..255)");
            }
        }

        //Błędy
        }
        else {
            Serial.println("ERR: Unknown command");
        }
    }
}

void loop() {
    unsigned long now = millis();
    if (now - lastTick >= TEMP_UPDATE_MS) {
        lastTick = now;
        fanCtrl.update();
    }
    
    // Obsługa komunikacji w czasie wolnym procesora
    handleCli();
}