# 📘 Reklamtavla i AVR-C

Detta projekt implementerar en reklamtavla i ren C för **ATmega328P**, helt utan Arduino-bibliotek. LCD-displayen styrs direkt via `PORTD` med en egenskreven 4-bitars HD44780-drivrutin. Projektet kompileras med `avr-gcc` via en Makefile och kan enkelt simuleras i Wokwi genom att ladda upp den genererade `.hex`-filen.

🔗 **GitHub Repo:** https://github.com/yosefammin11-yeye/Inlamningsuppgift-embedded
🖥️ **Wokwi Project:** https://wokwi.com/projects/456397236646087681


---

## 🔌 Hårdvara (Wokwi-koppling)
Projektet använder en parallell **LCD1602**. Drivern i `lcd.c` är skriven för exakt nedanstående koppling mot mikrokontrollerns D-port.

| LCD-pin | Arduino-pin (AVR-port) | Beskrivning |
| :--- | :--- | :--- |
| **RS** | D0 (PD0) | Register Select |
| **EN** | D1 (PD1) | Enable |
| **D4** | D4 (PD4) | Data 4 |
| **D5** | D5 (PD5) | Data 5 |
| **D6** | D6 (PD6) | Data 6 |
| **D7** | D7 (PD7) | Data 7 |
| **RW** | GND | Read/Write (Kopplas till jord) |
| **VSS** | GND | Jord |
| **VDD** | 5V | Strömförsörjning |
| **VO** | GND | Kontrast |

---

## 🛠 Kompilering
För att bygga projektet lokalt, öppna terminalen i projektmappen och kör:

```bash
make clean
make


