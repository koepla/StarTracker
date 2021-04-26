#ifndef STT_TMC2209_H
#define STT_TMC2209_H

#include <TMCStepper.h>

#define R_SENSE     		    0.11f
#define DRIVER_ADDRESS          0b00
#define MICROSTEPS              256
#define STEPS_PER_REV           51200

//yaw stepepr driver pins
#define ENABLE_PIN_YAW	        4       
#define DIRECTION_PIN_YAW       3		
#define STEP_PIN_YAW	        2
#define RX_PIN_YAW			    6
#define TX_PIN_YAW			    5

//pitch stepper driver1 pins
#define ENABLE_PIN_PITCH1		4      
#define DIRECTION_PIN_PITCH1	3		
#define STEP_PIN_PITCH1		    2
#define RX_PIN_PITCH1           6
#define TX_PIN_PITCH1	        5

//pitch stepper driver2 pins
#define ENABLE_PIN_PITCH2		4       
#define DIRECTION_PIN_PITCH2	3		 
#define STEP_PIN_PITCH2		    2
#define RX_PIN_PITCH2           6
#define TX_PIN_PITCH2	        5

TMC2209Stepper stepper_yaw(RX_PIN_YAW, TX_PIN_YAW, R_SENSE, DRIVER_ADDRESS);
TMC2209Stepper stepper_pitch1(RX_PIN_PITCH1, TX_PIN_PITCH1, R_SENSE, DRIVER_ADDRESS);
TMC2209Stepper stepper_pitch2(RX_PIN_PITCH2, TX_PIN_PITCH2, R_SENSE, DRIVER_ADDRESS);

#endif