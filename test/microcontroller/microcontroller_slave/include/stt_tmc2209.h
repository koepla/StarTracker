#ifndef STT_TMC2209_H
#define STT_TMC2209_H

#include <TMCStepper.h>
#include <Arduino.h>

#define R_SENSE     		    0.11f
#define DRIVER_ADDRESS          0b00
#define MICROSTEPS              256
#define STEPS_PER_REV           51200
#define DEFAULT_DIR             false

//yaw stepepr driver pins
#define ENABLE_PIN_YAW	        11     	
#define STEP_PIN_YAW	        10
#define RX_PIN_YAW			    13
#define TX_PIN_YAW			    12

//pitch stepper driver1 pins
#define ENABLE_PIN_PITCH1		3      	
#define STEP_PIN_PITCH1		    2
#define RX_PIN_PITCH1           5
#define TX_PIN_PITCH1	        4

//pitch stepper driver2 pins
#define ENABLE_PIN_PITCH2		7       	 
#define STEP_PIN_PITCH2		    6
#define RX_PIN_PITCH2           9
#define TX_PIN_PITCH2	        8

TMC2209Stepper stepper_yaw(RX_PIN_YAW, TX_PIN_YAW, R_SENSE, DRIVER_ADDRESS);
TMC2209Stepper stepper_pitch1(RX_PIN_PITCH1, TX_PIN_PITCH1, R_SENSE, DRIVER_ADDRESS);
TMC2209Stepper stepper_pitch2(RX_PIN_PITCH2, TX_PIN_PITCH2, R_SENSE, DRIVER_ADDRESS);

void move(double pitch_deg){

    uint64_t pitch_steps;
    uint64_t yaw_steps;

    pitch_steps = STEPS_PER_REV / 360 * abs(pitch_deg);
    //yaw_steps = STEPS_PER_REV / 360 * yaw_deg;

    if(pitch_deg > 0){

        stepper_pitch1.shaft(!DEFAULT_DIR);
        stepper_pitch2.shaft(DEFAULT_DIR);

        for(int i=0; i<pitch_steps; i++){

            digitalWrite(STEP_PIN_PITCH1, HIGH);
            digitalWrite(STEP_PIN_PITCH2, HIGH);
            delayMicroseconds(5000);
            digitalWrite(STEP_PIN_PITCH2, LOW);
            digitalWrite(STEP_PIN_PITCH1, LOW);
            delayMicroseconds(5000);
        }
    }
    if(pitch_deg < 0){

        stepper_pitch1.shaft(DEFAULT_DIR);
        stepper_pitch2.shaft(!DEFAULT_DIR);

        for(int i=0; i<pitch_steps; i++){

            digitalWrite(STEP_PIN_PITCH1, HIGH);
            digitalWrite(STEP_PIN_PITCH2, HIGH);
            delayMicroseconds(5000);
            digitalWrite(STEP_PIN_PITCH2, LOW);
            digitalWrite(STEP_PIN_PITCH1, LOW);
            delayMicroseconds(5000);
        }
    }
}

#endif