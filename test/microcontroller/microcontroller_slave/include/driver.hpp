#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <TMCStepper.h>

#define R_SENSE     		    0.11f
#define DRIVER_ADDRESS          0b00
#define DEFAULT_DIR true

enum class MotorAxis : uint8_t {

    PITCH = (1 << 0),
    YAW = (1 << 1)
};

enum class MotorState : uint8_t {

    ON,
    OFF
};

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
        this->currentPitch = 0.0f;
        this->currentYaw = 0.0f;
    }

    void Init() {

        initMotor(&pitchLeft, &pitchLeftConf, rmsCurrent);
        initMotor(&pitchRight, &pitchRightConf, rmsCurrent);
        initMotor(&yaw, &yawConf, rmsCurrent);
    }

    void Move(float pitchAngle, float yawAngle) {

        if(pitchLeftConf.microSteps != pitchRightConf.microSteps) {

            // In this case (Edge case because it would be the programmers fault) we don't want to move because otherwise 
            // we would damage the 3D printed construction
            return;
        }

        float pitchBy = pitchAngle - currentPitch;
        float yawBy = yawAngle - currentYaw;

        int64_t pitchSteps = pitchBy * static_cast<float>(pitchLeftConf.microSteps) * (200.0f / 360.0f);
        int64_t yawSteps = yawBy * static_cast<float>(yawConf.microSteps) * (200.0f / 360.0f);

        bool pitchShaft = pitchSteps > 0 ? true : false;
        pitchLeft.shaft(pitchShaft);
        pitchRight.shaft(!pitchShaft);

        for(int64_t i = 0; i < abs(pitchSteps); i++) {

            stepMotor(&pitchLeftConf);
            stepMotor(&pitchRightConf);
        }

        yaw.shaft(yawSteps < 0 ? true : false);

        for(int64_t i = 0; i < abs(yawSteps); i++) {

            stepMotor(&yawConf);
        }

        currentPitch = pitchAngle;
        currentYaw = yawAngle;
    }

    void SetMotorState(MotorAxis axis, MotorState state) {

        bool enable = state == MotorState::ON;

        switch(axis) {

            case MotorAxis::PITCH: {

                enableMotor(&pitchLeftConf, enable);
                enableMotor(&pitchRightConf, enable);
                break;
            }
            case MotorAxis::YAW: {

                enableMotor(&yawConf, enable);
                break;
            }
        }
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

    static void enableMotor(DriverConfig* config, bool enable) {

        digitalWrite(config->enablePin, enable ? LOW : HIGH); // enable driver in hardware
    }
};

#endif // _DRIVER_H_