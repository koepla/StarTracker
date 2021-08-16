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
    digitalWrite(ENABLE_PIN_PITCH1, LOW);       
    stepper_pitch1.beginSerial(115200);         
    stepper_pitch1.begin();                     
    stepper_pitch1.toff(5);                     
    stepper_pitch1.rms_current(1000);            
    stepper_pitch1.microsteps(MICROSTEPS);   
    stepper_pitch1.pwm_autoscale(true);      

    pinMode(ENABLE_PIN_PITCH2, OUTPUT);
    pinMode(STEP_PIN_PITCH2, OUTPUT);
    digitalWrite(ENABLE_PIN_PITCH2, LOW);     
    stepper_pitch2.beginSerial(115200);    
    stepper_pitch2.begin();                 
    stepper_pitch2.toff(5);                     
    stepper_pitch2.rms_current(1000);          
    stepper_pitch2.microsteps(MICROSTEPS);    
    stepper_pitch2.pwm_autoscale(true);

    pinMode(ENABLE_PIN_YAW, OUTPUT);
    pinMode(STEP_PIN_YAW, OUTPUT);
    digitalWrite(ENABLE_PIN_YAW, LOW);     
    stepper_yaw.beginSerial(115200);    
    stepper_yaw.begin();                 
    stepper_yaw.toff(5);                     
    stepper_yaw.rms_current(600);          
    stepper_yaw.microsteps(MICROSTEPS);    
    stepper_yaw.pwm_autoscale(true);              

    Serial.begin(115200);
}

void loop(){

    if(Serial.available() == 40){

        Serial.readBytes(buff, 40);
        float pitch = pack.read<float>(0);
        float yaw = pack.read<float>(1);
        move(pitch, yaw);
    }
}
