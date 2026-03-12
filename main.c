#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h> // Tillagd för strlen() och strncpy()
#include "lcd.h"

// =========================
//  KONSTANTER
// =========================
#define VIEW_TIME_MS 20000   // 20 sekunder

// Kund-ID
#define HARRY   0
#define ANKA    1
#define PETTER  2
#define LANGBEN 3
#define IOT     4

// Viktning baserat på betalning (const lagt till för att spara RAM)
const int weights[] = { 5, 3, 2, 4, 1 };

// =========================
//  KUND-MEDDELANDEN
// =========================

// Harry (3 meddelanden)
const char* harry_msgs[] = {
    "Kop bil hos Harry",
    "En god bilaffar!",
    "Hederlige Harrys Bilar"
};

// Farmor Anka (2 meddelanden)
const char* anka_msgs[] = {
    "Kop paj hos Anka",
    "Skynda innan Morten ater!"
};

// Svarte Petter (2 meddelanden)
const char* petter_msgs[] = {
    "Lat Petter bygga at dig",
    "Bygga svart? Ring Petter"
};

// Langben (2 meddelanden)
const char* langben_msgs[] = {
    "Mysterier? Ring Langben",
    "Langben fixar biffen"
};

// IOT (1 meddelande)
const char* iot_msg = "Synas har? IOT Reklam";

// =========================
//  FUNKTIONER
// =========================

// Visar ett meddelande i 20 sekunder (statiskt)
void show_message(const char* msg) {
    lcd_clear();
    
    // SKAPAR EN KOPIA: Eftersom swedish_parser ändrar i strängen måste vi 
    // kopiera den till en lokal buffer, annars kraschar programmet när 
    // den försöker ändra i konstant ROM-minne.
    char buffer[32]; 
    strncpy(buffer, msg, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    lcd_puts(buffer);

    for (int i = 0; i < VIEW_TIME_MS / 100; i++) {
        _delay_ms(100);
    }
}

// Mjukvaru-scroll (smooth, hela texten visas)
void scroll_message(const char* msg) {
    int len = strlen(msg); // Använder strlen istället för while-loop

    // Om texten får plats → visa statiskt
    if (len <= 16) {
        show_message(msg);
        return;
    }

    char window[17];
    window[16] = '\0';

    // Scrolla hela texten + 16 steg för att scrolla ut den
    for (int pos = 0; pos < len + 16; pos++) {

        for (int i = 0; i < 16; i++) {
            int idx = pos + i;

            if (idx < len)
                window[i] = msg[idx];
            else
                window[i] = ' '; // Fyll på med mellanslag på slutet
        }

        lcd_clear();
        lcd_puts(window); // window är redan en lokal, ändringsbar buffer (säker!)
        _delay_ms(300);
    }

    // Räkna ut återstående tid
    int scroll_time = (len + 16) * 300;
    int remaining = VIEW_TIME_MS - scroll_time;
    
    if (remaining < 0) {
        remaining = 0;
    }

    for (int i = 0; i < remaining / 100; i++) {
        _delay_ms(100);
    }
}

// Initierar slumpgeneratorn
void init_random() {
    srand(12345); // stabil seed
}

// Viktad slumpning av kund
int weighted_pick() {
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += weights[i];
    }

    int r = rand() % total;
    int sum = 0;

    for (int i = 0; i < 5; i++) {
        sum += weights[i];
        if (r < sum) {
            return i;
        }
    }
    return 0;
}

// =========================
//  MAIN
// =========================
int main(void) {
    lcd_init();
    init_random();

    // Text i variabel så parsern inte kraschar programmet
    char start_msg[] = "Reklamtavla";
    lcd_puts(start_msg);
    _delay_ms(2000);

    int last = -1;
    int fake_minute = 0;

    while (1) {
        // Slumpa kund, men inte samma två gånger i rad
        int customer;
        do {
            customer = weighted_pick();
        } while (customer == last);

        last = customer;

        // Välj och visa meddelande
        switch (customer) {
            case HARRY: {
                int r = rand() % 3;
                show_message(harry_msgs[r]);
                break;
            }

            case ANKA: {
                int r = rand() % 2;
                show_message(anka_msgs[r]);
                break;
            }

            case PETTER: {
                int r = rand() % 2;

                // Jämn "minut" → scroll, ojämn → statiskt
                if (fake_minute % 2 == 0) {
                    scroll_message(petter_msgs[r]);
                } else {
                    show_message(petter_msgs[r]);
                }
                break;
            }

            case LANGBEN: {
                int r = rand() % 2;
                show_message(langben_msgs[r]);
                break;
            }

            case IOT:
                show_message(iot_msg);
                break;
        }

        fake_minute++;
    }

    return 0;
}