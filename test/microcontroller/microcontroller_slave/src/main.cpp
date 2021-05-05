#include <Arduino.h>
#include "stt_package.h"
#include "stt_tmc2209.h"

stt_package<32> pack;
uint8_t* buff = reinterpret_cast<uint8_t*>(&pack);
bool shaft = false;

void turn(float angle);

void setup(){

    pinMode(ENABLE_PIN_PITCH1, OUTPUT);
    pinMode(STEP_PIN_PITCH1, OUTPUT);
    digitalWrite(ENABLE_PIN_PITCH1, LOW);       // Enable driver in hardware
    stepper_pitch1.beginSerial(115200);         // SW UART drivers
    stepper_pitch1.begin();                     //  SPI: Init CS pins and possible SW SPI pins
    stepper_pitch1.toff(5);                     // Enables driver in software
    stepper_pitch1.rms_current(600);            // Set motor RMS current
    stepper_pitch1.microsteps(MICROSTEPS);      // Set microsteps to 1/16th
    stepper_pitch1.pwm_autoscale(true);         // Needed for stealthChop

    Serial.begin(115200);
}

void loop(){

    if(Serial.available() == 40){

        Serial.readBytes(buff, 40);
        float angle = pack.read<float>(0);
        move(angle);
    }
}
