#ifndef STARTRACKER_FIRMWARE_DRIVER_H
#define STARTRACKER_FIRMWARE_DRIVER_H

#include <TMCStepper.h>

#define R_SENSE 0.11f
#define DRIVER_ADDRESS 0x00

enum class MotorAxis : uint8_t {

    PITCH = (1 << 0),
    YAW = (1 << 1)
};

enum class MotorState : uint8_t {

    ON,
    OFF
};

struct DriverConfig {

    uint16_t MicroSteps;
    uint16_t EnablePin;
    uint16_t StepPin;
    uint16_t RxPin;
    uint16_t TxPin;
    float RmsCurrent;
};

class Driver {

private:
    DriverConfig pitchLeftConf;
    DriverConfig pitchRightConf;
    DriverConfig yawConf;

    TMC2209Stepper pitchLeft;
    TMC2209Stepper pitchRight;
    TMC2209Stepper yaw;

    float currentPitch;
    float currentYaw;

public:
    Driver(DriverConfig pitchLeftConfig, DriverConfig pitchRightConfig, DriverConfig yawConfig) 
    : 
        pitchLeft{TMC2209Stepper{pitchLeftConfig.RxPin, pitchLeftConfig.TxPin, R_SENSE, DRIVER_ADDRESS}},
        pitchRight{TMC2209Stepper{pitchRightConfig.RxPin, pitchRightConfig.TxPin, R_SENSE, DRIVER_ADDRESS}},
        yaw{TMC2209Stepper{yawConfig.RxPin, yawConfig.TxPin, R_SENSE, DRIVER_ADDRESS}}
    { 
        this->pitchLeftConf = pitchLeftConfig;
        this->pitchRightConf = pitchRightConfig;
        this->yawConf = yawConfig;

        this->currentPitch = 0.0f;
        this->currentYaw = 0.0f;
    }

    void Init() {

        initMotor(&pitchLeft, &pitchLeftConf);
        initMotor(&pitchRight, &pitchRightConf);
        initMotor(&yaw, &yawConf);
    }

    void Move(float pitchAngle, float yawAngle) {

        if(pitchLeftConf.MicroSteps != pitchRightConf.MicroSteps) {

            // In this case (Edge case because it would be the programmers fault) we don't want to move because otherwise 
            // we would damage the 3D printed construction
            return;
        }

        float pitchBy = pitchAngle - currentPitch;
        float yawBy = yawAngle - currentYaw;

        int64_t pitchSteps = pitchBy * static_cast<float>(pitchLeftConf.MicroSteps) * (200.0f / 360.0f);
        int64_t yawSteps = yawBy * static_cast<float>(yawConf.MicroSteps) * (200.0f / 360.0f);

        bool pitchShaft = pitchSteps > 0 ? true : false;
        pitchLeft.shaft(!pitchShaft);
        pitchRight.shaft(pitchShaft);
        yaw.shaft(yawSteps < 0 ? true : false);

        const int64_t aPitchSteps = abs(pitchSteps);
        const int64_t aYawSteps = abs(yawSteps);

        for(int64_t i = 0; i < max(aPitchSteps, aYawSteps); i++) {

            if(i < aPitchSteps) {

                stepMotor(&pitchLeftConf, &pitchRightConf, 100, 100);
            }
            if(i < aYawSteps) {

                stepMotor(&yawConf, nullptr, 100, 100);
            }
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

    void SetCurrentPosition(float pitchAngle, float yawAngle) {

        this->currentPitch = pitchAngle;
        this->currentYaw = yawAngle;
    }


private:
    static void initMotor(TMC2209Stepper* motor, DriverConfig* config) {

        pinMode(config->EnablePin, OUTPUT);
        pinMode(config->StepPin, OUTPUT);
        
        digitalWrite(config->EnablePin, LOW); // enable driver in hardware

        motor->beginSerial(115200);
        motor->begin();
        motor->toff(5); // enable driver in software
        motor->rms_current(config->RmsCurrent);
        motor->microsteps(config->MicroSteps);
        motor->pwm_autoscale(true);
    }

    static void stepMotor(DriverConfig* config, DriverConfig* secondConfig = nullptr, uint32_t delayUsBeg = 1000, uint32_t delayUsEnd = 1000) {

        digitalWrite(config->StepPin, HIGH);

        if(secondConfig != nullptr) {

            digitalWrite(secondConfig->StepPin, HIGH);
        }

        delayMicroseconds(delayUsBeg);

        digitalWrite(config->StepPin, LOW);

        if(secondConfig != nullptr) {

            digitalWrite(secondConfig->StepPin, LOW);
        }

        delayMicroseconds(delayUsEnd);
    }

    static void enableMotor(DriverConfig* config, bool enable) {

        digitalWrite(config->EnablePin, enable ? LOW : HIGH); // enable driver in hardware
    }
};

#endif // STARTRACKER_FIRMWARE_DRIVER_H