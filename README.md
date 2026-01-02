# Smart Fan Controller (ESP32)

System sterowania wentylatorem oparty o temperaturę z funkcją bezpieczeństwa (Safe Mode).

## Struktura
- **app/**: Logika sterowania (LUT, Rampa, FSM).
- **drivers/**: Warstwa sprzętowa (ADC, PWM, GPIO).
- **src/**: Główna pętla programu i CLI.

## Funkcjonalność
1. **Sterowanie:** PWM zależne od temperatury.
2. **Safety:** Wykrywanie uszkodzenia czujnika (Short/Open) -> Alarm (LED+Buzzer) + Max Fan.
3. **CLI:** Komendy `STAT` (telemetria) i `OFFSET` (kalibracja) przez UART.

## Uruchomienie (Wokwi)
1. Załaduj pliki z  Projekt Final Files from WOKWI.zip do WOKWI.
2. Skompiluj kod źródłowy zielonym przyciskiem.
