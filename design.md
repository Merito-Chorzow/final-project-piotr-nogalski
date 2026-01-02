# Architektura Systemu

## Model Warstwowy
1. **Application Layer (`FanController`)**:
   - Decyduje "CO" ma się stać (np. włącz alarm, ustaw obroty na 50%).
   - Nie zależy od sprzętu (testowalne).
2. **Driver Layer (`TempSensor`, `FanMotor`, `Buzzer`)**:
   - Wie "JAK" to zrobić (np. wpisz 128 do rejestru LEDC na pinie 2).
   - Zależy od frameworka Arduino/ESP32.

## Maszyna Stanów (FSM)
- **NORMAL**: Odczyt Temp -> Obliczenie PWM (LUT) -> Rampa -> Wyjście.
- **FAULT**: Temp poza zakresem -> PWM = 100%, LED_FAULT = ON, BUZZER = ON.

## Algorytm Rampy
`Duty` nie jest przepisywane wprost. W każdej pętli (100ms) aktualne wypełnienie zbliża się do celu o `RAMP_STEP` (5 jednostek).