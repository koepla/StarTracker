#include "stepper-driver.hpp"

#define R_SENSE 0.11f
#define DRIVER_ADDRESS 0x00

namespace StarTracker {

    namespace {

        void InitializeStepper(TMC2209Stepper& stepper, const StepperDriverConfig& config) noexcept {
            pinMode(config.EnablePin, OUTPUT);
            pinMode(config.StepPin, OUTPUT);

            // Enable StepperDriver in Hardware
            digitalWrite(config.EnablePin, LOW);
            stepper.beginSerial(115200);
            stepper.begin();
            stepper.toff(5);
            stepper.rms_current(config.RmsCurrent, 1.0f);
            stepper.microsteps(config.Microsteps);
            stepper.pwm_autoscale(true);
        }

        void UpdateRmsCurrent(TMC2209Stepper& stepper, StepperDriverConfig& config, float rmsCurrent) noexcept {
            config.RmsCurrent = rmsCurrent;
            stepper.rms_current(config.RmsCurrent, 1.0f);
        }

        void UpdateMicroSteps(TMC2209Stepper& stepper, StepperDriverConfig& config, uint16_t microsteps) noexcept {
            config.Microsteps = microsteps;
            stepper.microsteps(config.Microsteps);
        }

        void EnableStepper(const StepperDriverConfig& config, bool enable) noexcept {
            digitalWrite(config.EnablePin, enable ? LOW : HIGH);
        }

        void PerformStep(const StepperDriverConfig& config) noexcept {
            digitalWrite(config.StepPin, HIGH);
            delayMicroseconds(1);
            digitalWrite(config.StepPin, LOW);
            delayMicroseconds(1);
        }

        template<typename GenType>
        GenType Abs(GenType x) {
            if (x < 0) {
                return -x;
            } else {
                return x;
            }
        }

        template<typename GenType>
        GenType Min(GenType a, GenType b) {
            if (a < b) {
                return a;
            } else {
                return b;
            }
        }

        template<typename GenType>
        GenType Max(GenType a, GenType b) {
            if (a > b) {
                return a;
            } else {
                return b;
            }
        }
    }// namespace

    StepperDriver::StepperDriver(const StepperDriverConfig& leftPitchConfig,
                                 const StepperDriverConfig& rightPitchConfig,
                                 const StepperDriverConfig& yawConfig) noexcept
        : leftPitchConfig{ leftPitchConfig },
          rightPitchConfig{ rightPitchConfig },
          yawConfig{ yawConfig },
          leftPitchStepper{ leftPitchConfig.RxPin, leftPitchConfig.TxPin, R_SENSE, DRIVER_ADDRESS },
          rightPitchStepper{ rightPitchConfig.RxPin, rightPitchConfig.TxPin, R_SENSE, DRIVER_ADDRESS },
          yawStepper{ yawConfig.RxPin, yawConfig.TxPin, R_SENSE, DRIVER_ADDRESS },
          currentPitchAngle{ 0.0f },
          currentYawAngle{ 0.0f } { }

    void StepperDriver::Begin() {
        InitializeStepper(leftPitchStepper, leftPitchConfig);
        InitializeStepper(rightPitchStepper, rightPitchConfig);
        InitializeStepper(yawStepper, yawConfig);
    }

    void StepperDriver::MoveToTarget(float pitchAngle, float yawAngle, float angularSpeed) noexcept {
        const auto pitchDiff = pitchAngle - currentPitchAngle;
        const auto yawDiff = yawAngle - currentYawAngle;
        const auto maxAngle = Max(Abs(pitchDiff), Abs(yawDiff));

        // This tells us how many steps are required per degree
        const auto stepsPerDegree =
                leftPitchConfig.GearRatio * static_cast<float>(leftPitchConfig.Microsteps) * 200.0f / 360.0f;
        auto pitchSteps = static_cast<int32_t>(pitchDiff * stepsPerDegree);
        auto yawSteps = static_cast<int32_t>(yawDiff * stepsPerDegree);

        // Shaft to switch direction when steps are negative
        const auto pitchShaft = pitchSteps > 0;
        leftPitchStepper.shaft(!pitchShaft);
        rightPitchStepper.shaft(pitchShaft);
        yawStepper.shaft(yawSteps < 0);

        pitchSteps = Abs(pitchSteps);
        yawSteps = Abs(yawSteps);
        const auto maxSteps = Max(pitchSteps, yawSteps);

        const auto totalTime = maxAngle / angularSpeed;
        const auto timePerStep = 1e6f * totalTime / static_cast<float>(maxSteps);

        for (int32_t i = 0; i < maxSteps; i++) {
            if (i < pitchSteps) {
                PerformStep(leftPitchConfig);
                PerformStep(rightPitchConfig);
            } 
            if (i < yawSteps) {
                PerformStep(yawConfig);
            }
            delayMicroseconds(timePerStep - 20);
        }

        // Only if the angle was big enough for at least one step, we change the internal angle
        currentPitchAngle = pitchSteps > 0 ? pitchAngle : currentPitchAngle;
        currentYawAngle = yawSteps > 0 ? yawAngle : currentYawAngle;
    }

    void StepperDriver::SetCurrentPosition(float pitchAngle, float yawAngle) noexcept {
        currentPitchAngle = pitchAngle;
        currentYawAngle = yawAngle;
    }

    void StepperDriver::SetMotorState(MotorAxis motorAxis, MotorState motorState) noexcept {
        const auto enable = motorState == MotorState::On;
        switch (motorAxis) {
            case MotorAxis::Pitch: {
                EnableStepper(leftPitchConfig, enable);
                EnableStepper(rightPitchConfig, enable);
                break;
            }
            case MotorAxis::Yaw: {
                EnableStepper(yawConfig, enable);
                break;
            }
        }
    }

    void StepperDriver::SetGearRatio(float ratio) noexcept {
        yawConfig.GearRatio = ratio;
        leftPitchConfig.GearRatio = ratio;
        rightPitchConfig.GearRatio = ratio;
    }

    void StepperDriver::SetRmsCurrent(float rmsCurrent) noexcept {
        UpdateRmsCurrent(leftPitchStepper, leftPitchConfig, rmsCurrent);
        UpdateRmsCurrent(rightPitchStepper, rightPitchConfig, rmsCurrent);
        UpdateRmsCurrent(yawStepper, yawConfig, rmsCurrent);
    }

    void StepperDriver::SetMicroSteps(float microSteps) noexcept {
        const auto steps = static_cast<uint16_t>(microSteps);
        UpdateMicroSteps(leftPitchStepper, leftPitchConfig, steps);
        UpdateMicroSteps(rightPitchStepper, rightPitchConfig, steps);
        UpdateMicroSteps(yawStepper, yawConfig, steps);
    }

    float StepperDriver::GetCurrentPitch() const noexcept {
        return currentPitchAngle;
    }

    float StepperDriver::GetCurrentYaw() const noexcept {
        return currentYawAngle;
    }
}// namespace StarTracker