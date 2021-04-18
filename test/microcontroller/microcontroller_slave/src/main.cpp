#include <Arduino.h>
#include "LiquidCrystal.h"
#include "stt_protocol.h"

const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
stt_protocol<32> prot;
uint8_t* buffer = reinterpret_cast<uint8_t*>(&prot);

void setup(){
    Serial.begin(256000);
    lcd.begin(16, 2);
}

void loop(){

    if(Serial.available() == 40){

        Serial.readBytes(buffer, 40); 

        for(int i = 0; i < 8; i++){

            float x = prot.read<float>(i);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(i);
            lcd.setCursor(0, 1);
            lcd.print(x);

            delay(100);
        }
    }
    else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("available: ");
        lcd.setCursor(0, 1);
        lcd.print(Serial.available());
        delay(100);
    }
}