#ifndef STARTRACKER_FIRMWARE_STEPPERDRIVER_H
#define STARTRACKER_FIRMWARE_STEPPERDRIVER_H

#include <TMCStepper.h>

namespace StarTracker {

    enum class MotorAxis : uint8_t {

        PITCH = (1 << 0),
        YAW = (1 << 1)
    };

    enum class MotorState : uint8_t {

        ON,
        OFF
    };

    struct StepperDriverConfig {

        uint16_t Microsteps;
        uint16_t EnablePin;
        uint16_t StepPin;
        uint16_t RxPin;
        uint16_t TxPin;
        float RmsCurrent;
    };

    class StepperDriver {

    private:
        StepperDriverConfig leftPitchConfig;
        StepperDriverConfig rightPitchConfig;
        StepperDriverConfig yawConfig;

        TMC2209Stepper leftPitchStepper;
        TMC2209Stepper rightPitchStepper;
        TMC2209Stepper yawStepper;

        float currentPitchAngle;
        float currentYawAngle;

    public:
        StepperDriver(const StepperDriverConfig& leftPitchConfig, const StepperDriverConfig& rightPitchConfig, const StepperDriverConfig& yawConfig) noexcept;
        
        void MoveToTarget(float pitchAngle, float yawAngle, size_t durationMs) noexcept;
        void SetCurrentPosition(float pitchAngle, float yawAngle) noexcept;
        void SetMotorState(MotorAxis motorAxis, MotorState motorState) noexcept;

        [[nodiscard]] float GetCurrentPitch() const noexcept;
        [[nodiscard]] float GetCurrentYaw() const noexcept;

    private:
        static void initStepper(TMC2209Stepper& stepper, const StepperDriverConfig& config) noexcept;
        static void enableStepper(const StepperDriverConfig& config, bool enable) noexcept;
        static void step(const StepperDriverConfig& config, size_t durationUs) noexcept;
    };
}



#endif // STARTRACKER_FIRMWARE_STEPPERDRIVER_H