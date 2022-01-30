#include "StepperDriver.hpp"

#define R_SENSE 0.11f
#define DRIVER_ADDRESS 0x00

namespace StarTracker {

    StepperDriver::StepperDriver(const StepperDriverConfig& leftPitchConfig, const StepperDriverConfig& rightPitchConfig, const StepperDriverConfig& yawConfig) noexcept :
    leftPitchConfig{leftPitchConfig},
    rightPitchConfig{rightPitchConfig},
    yawConfig{yawConfig},
    leftPitchStepper{leftPitchConfig.RxPin, leftPitchConfig.TxPin, R_SENSE, DRIVER_ADDRESS},
    rightPitchStepper{rightPitchConfig.RxPin, rightPitchConfig.TxPin, R_SENSE, DRIVER_ADDRESS},
    yawStepper{yawConfig.RxPin, yawConfig.TxPin, R_SENSE, DRIVER_ADDRESS},
    currentPitchAngle{0.0f},
    currentYawAngle{0.0f} {

        initStepper(leftPitchStepper, leftPitchConfig);
        initStepper(rightPitchStepper, rightPitchConfig);
        initStepper(yawStepper, yawConfig);
    }
        
    void StepperDriver::MoveToTarget(float pitchAngle, float yawAngle, size_t durationMs) noexcept {

        if(leftPitchConfig.Microsteps != rightPitchConfig.Microsteps) {

            // In this case (Edge case because it would be the programmers fault) we don't want to move because otherwise 
            // we would damage the 3D printed construction
            return;
        }

        const auto pitchSteps = static_cast<int64_t>((pitchAngle - currentPitchAngle) * static_cast<float>(leftPitchConfig.Microsteps) * (200.0f / 360.0f));
        const auto yawSteps = static_cast<int64_t>((yawAngle - currentYawAngle) * static_cast<float>(yawConfig.Microsteps) * (200.0f / 360.0f));
    
        const auto pitchShaft = pitchSteps > 0;
        leftPitchStepper.shaft(!pitchSteps);
        rightPitchStepper.shaft(pitchShaft);
        yawStepper.shaft(yawSteps < 0);

        const auto absolutePitchSteps = abs(pitchSteps);
        const auto absoluteYawSteps = abs(yawSteps);
        const auto maxSteps = max(absolutePitchSteps, absoluteYawSteps);

        const auto stepDuration = (static_cast<float>(durationMs) * 1000.0f) / static_cast<float>(maxSteps);

        for(auto i = int64_t{ 0 }; i < maxSteps; i++) {

            // Takes stepDuration / 2 Milliseconds
            if(i < absolutePitchSteps) {

                step(leftPitchConfig, static_cast<size_t>(stepDuration / 4.0f));
                step(rightPitchConfig, static_cast<size_t>(stepDuration / 4.0f));
            }

            // Takes stepDuration / 2 Milliseconds
            if(i < absoluteYawSteps) {

                step(yawConfig, static_cast<size_t>(stepDuration / 2.0f));
            }
        }

        currentPitchAngle = pitchAngle;
        currentYawAngle = yawAngle;
    }

    void StepperDriver::SetCurrentPosition(float pitchAngle, float yawAngle) noexcept {

        currentPitchAngle = pitchAngle;
        currentYawAngle = yawAngle;
    }

    void StepperDriver::SetMotorState(MotorAxis motorAxis, MotorState motorState) noexcept {

        const auto enable = motorState == MotorState::ON;

        switch(motorAxis) {

            case MotorAxis::PITCH: {

                enableStepper(leftPitchConfig, enable);
                enableStepper(rightPitchConfig, enable);
                break;
            }
            case MotorAxis::YAW: {

                enableStepper(yawConfig, enable);
                break;
            }
            default: {

                break;
            }
        }
    }

    float StepperDriver::GetCurrentPitch() const noexcept {

        return currentPitchAngle;
    }

    float StepperDriver::GetCurrentYaw() const noexcept {

        return currentYawAngle;
    }

    void StepperDriver::initStepper(TMC2209Stepper& stepper, const StepperDriverConfig& config) noexcept {

        pinMode(config.EnablePin, OUTPUT);
        pinMode(config.StepPin, OUTPUT);

        // Enable StepperDriver in Hardware
        digitalWrite(config.EnablePin, LOW);

        stepper.beginSerial(115200);
        stepper.begin();
        stepper.toff(5);
        stepper.rms_current(config.RmsCurrent);
        stepper.microsteps(config.Microsteps);
        stepper.pwm_autoscale(true);
    }

    void StepperDriver::enableStepper(const StepperDriverConfig& config, bool enable) noexcept {

        digitalWrite(config.EnablePin, enable ? LOW : HIGH);
    }

    void StepperDriver::step(const StepperDriverConfig& config, size_t durationUs) noexcept {

        const auto delayUs = (durationUs - 7) / 2;

        digitalWrite(config.StepPin, HIGH);        
        delayMicroseconds(delayUs);
        digitalWrite(config.StepPin, LOW);
        delayMicroseconds(delayUs);
    }
}