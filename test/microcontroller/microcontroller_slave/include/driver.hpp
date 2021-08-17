#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <TMCStepper.h>

#define R_SENSE     		    0.11f
#define DRIVER_ADDRESS          0b00
#define DEFAULT_DIR true

struct DriverConfig {

    uint16_t microSteps;
    uint16_t enablePin;
    uint16_t stepPin;
    uint16_t rxPin;
    uint16_t txPin;
};

class Driver {

private:

    DriverConfig pitchLeftConf;
    DriverConfig pitchRightConf;
    DriverConfig yawConf;

    TMC2209Stepper pitchLeft;
    TMC2209Stepper pitchRight;
    TMC2209Stepper yaw;

    float rmsCurrent;
    float currentPitch;
    float currentYaw;

public:
    Driver(DriverConfig pitchLeftConfig, DriverConfig pitchRightConfig, DriverConfig yawConfig, float rmsCurrent = 600) 
    : 
        pitchLeft(TMC2209Stepper(pitchLeftConfig.rxPin, pitchLeftConfig.txPin, R_SENSE, DRIVER_ADDRESS)),
        pitchRight(TMC2209Stepper(pitchRightConfig.rxPin, pitchRightConfig.txPin, R_SENSE, DRIVER_ADDRESS)),
        yaw(TMC2209Stepper(yawConfig.rxPin, yawConfig.txPin, R_SENSE, DRIVER_ADDRESS))
    { 
        this->pitchLeftConf = pitchLeftConfig;
        this->pitchRightConf = pitchRightConfig;
        this->yawConf = yawConfig;

        this->rmsCurrent = rmsCurrent;
        this->currentPitch = 0;
        this->currentYaw = 0;
    }

    void Init() {

        initMotor(&pitchLeft, &pitchLeftConf, rmsCurrent);
        initMotor(&pitchRight, &pitchRightConf, rmsCurrent);
        initMotor(&yaw, &yawConf, rmsCurrent);
    }

    void Move(float pitchAngle, float yawAngle) {

        float pitchBy = pitchAngle - currentPitch;
        float yawBy = yawAngle - currentYaw;

        float stepsPerDegreePitch = pitchLeftConf.microSteps * (200.0 / 360.0);
        float stepsPerDegreeYaw = yawConf.microSteps * (200.0 / 360.0);

        uint64_t pitchSteps =  stepsPerDegreePitch * abs(pitchBy);
        uint64_t yawSteps =  stepsPerDegreeYaw * abs(yawBy);

        if(pitchAngle > currentPitch){

            pitchLeft.shaft(!DEFAULT_DIR);
            pitchRight.shaft(DEFAULT_DIR);
        }
        else if(pitchAngle < currentPitch) {

            pitchLeft.shaft(DEFAULT_DIR);
            pitchRight.shaft(!DEFAULT_DIR);
        }

        for(uint64_t i = 0; i < pitchSteps; i++){

            stepMotor(&pitchLeftConf);
            stepMotor(&pitchRightConf);
        }

        if(yawAngle > currentYaw){

            yaw.shaft(DEFAULT_DIR);
        }
        if(yawAngle < currentYaw){

            yaw.shaft(DEFAULT_DIR);
        }

        for(uint64_t i = 0; i < yawSteps; i++) {

            stepMotor(&yawConf);
        }

        currentPitch = pitchAngle;
        currentYaw = yawAngle;
    }


private:
    static void initMotor(TMC2209Stepper* motor, DriverConfig* config, float rmsCurrent) {

        pinMode(config->enablePin, OUTPUT);
        pinMode(config->stepPin, OUTPUT);
        
        digitalWrite(config->enablePin, LOW); // enable driver in hardware

        motor->beginSerial(115200);
        motor->begin();
        motor->toff(5); // enable driver in software
        motor->rms_current(rmsCurrent);
        motor->microsteps(config->microSteps);
        motor->pwm_autoscale(true);
    }

    static void stepMotor(DriverConfig* config, uint32_t delayUs = 160) {

        digitalWrite(config->stepPin, HIGH);
        delayMicroseconds(delayUs);
        digitalWrite(config->stepPin, LOW);
        delayMicroseconds(delayUs);
    }

};

#endif // _DRIVER_H_