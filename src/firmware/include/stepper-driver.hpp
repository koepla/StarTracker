#ifndef STARTRACKER_FIRMWARE_STEPPERDRIVER_H
#define STARTRACKER_FIRMWARE_STEPPERDRIVER_H

#include <TMCStepper.h>

namespace StarTracker {

    enum class MotorAxis : uint8_t { Pitch = (1 << 0), Yaw = (1 << 1) };

    enum class MotorState : uint8_t { On, Off };

    struct StepperDriverConfig {
        uint16_t Microsteps;
        uint16_t EnablePin;
        uint16_t StepPin;
        uint16_t RxPin;
        uint16_t TxPin;
        float RmsCurrent;
        float GearRatio;
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
        StepperDriver(const StepperDriverConfig& leftPitchConfig,
                      const StepperDriverConfig& rightPitchConfig,
                      const StepperDriverConfig& yawConfig) noexcept;

        void Begin() noexcept;

        /**
         * @brief Points the Tracking-Mount to the specified position
         * @param pitchAngle Pitch (Altitude)
         * @param yawAngle Yaw (Azimuth)
         * @param angularSpeed Angular speed for the rotation
         */
        void MoveToTarget(float pitchAngle, float yawAngle, float angularSpeed) noexcept;

        /**
         * @brief Set the Current Position of the Tracking-Mount
         * @param pitchAngle Pitch (Altitude)
         * @param yawAngle Yaw (Azimuth)
         */
        void SetCurrentPosition(float pitchAngle, float yawAngle) noexcept;

        /**
         * @brief Set the Motor State
         * @param motorAxis Axis
         * @param motorState State
         */
        void SetMotorState(MotorAxis motorAxis, MotorState motorState) noexcept;

        /**
         * @brief Set the Gear-Ratio
         * @param ratio Gear-Ratio
         */
        void SetGearRatio(float ratio) noexcept;

        /**
         * @brief Set the Rms Current
         * @param rmsCurrent Current in milli ampere
         */
        void SetRmsCurrent(float rmsCurrent) noexcept;

        /**
         * @brief Set the Micro Steps
         * @param microSteps Micro steps
         */
        void SetMicroSteps(float microSteps) noexcept;

        /**
         * @brief Get the current pitch angle
         * @return Pitch in degrees
         */
        float GetCurrentPitch() const noexcept;

        /**
         * @brief Get the current yaw angle
         * @return Yaw in degrees
         */
        float GetCurrentYaw() const noexcept;
    };
}// namespace StarTracker


#endif// STARTRACKER_FIRMWARE_STEPPERDRIVER_H