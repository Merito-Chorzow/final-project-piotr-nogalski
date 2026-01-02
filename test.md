# Scenariusze Testowe

| ID | Test Case | Akcja | Oczekiwany Wynik | Status |
|----|-----------|-------|------------------|--------|
| T1 | **Cold Start** | Potencjometr na min (<20C) | Fan LED wyłączona. Fault LED wyłączona. | PASS |
| T2 | **Ramping** | Szybki ruch potencjometrem 20C -> 70C | Wartość STAT rośnie płynnie | PASS |
| T3 | **Fault High** | Odłączenie czujnika (Potencjometr MAX) | Fan LED = 100%, Fault LED = ON, Buzzer = ON. | PASS |
| T4 | **Fault Low** | Zwarcie czujnika (Potencjometr MIN) | Fan LED = 100%, Fault LED = ON, Buzzer = ON. | PASS |
| T5 | **Telemetry** | Wpisanie `STAT` w terminal | Zwraca wartości | PASS |
| T6 | **Telemetry** | Wpisanie `OFFSET 30` w terminal | Zmienia PWN z 0 na 30. | PASS |
| T7 | **Telemetry** | Wpisanie `HELP` w terminal | Zwraca instrukcje komend| PASS |