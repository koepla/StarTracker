#include <Arduino.h>

#include "package.hpp"
#include "stepper-driver.hpp"

using namespace StarTracker;

StepperDriverConfig leftPitch{ .Microsteps = 32,
                               .EnablePin = 3,
                               .StepPin = 2,
                               .RxPin = 5,
                               .TxPin = 4,
                               .RmsCurrent = 1200.0f,
                               .GearRatio = 1.0f };

StepperDriverConfig rightPitch{ .Microsteps = 32,
                                .EnablePin = 7,
                                .StepPin = 6,
                                .RxPin = 9,
                                .TxPin = 8,
                                .RmsCurrent = 1200.0f,
                                .GearRatio = 1.0f };

StepperDriverConfig yaw{ .Microsteps = 32,
                         .EnablePin = 11,
                         .StepPin = 10,
                         .RxPin = 13,
                         .TxPin = 12,
                         .RmsCurrent = 1200.0f,
                         .GearRatio = 1.0f };

StepperDriver stepperDriver{ leftPitch, rightPitch, yaw };

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        // Wait for Serial to establish connection
    }
    stepperDriver.Begin();
}

void loop() {
    Pack32 package{};
    Pack32 acknowledgePackage{ Command::Ack };

    if (static_cast<size_t>(Serial.available()) < sizeof(package)) {
        delay(50);
        return;
    }

    Serial.readBytes(reinterpret_cast<uint8_t*>(&package), sizeof(package));

    switch (package.GetFlag()) {
        case Command::None:
            break;
        case Command::Wakeup:
            stepperDriver.SetMotorState(package.Read<MotorAxis>(0), MotorState::On);
            break;
        case Command::Sleep:
            stepperDriver.SetMotorState(package.Read<MotorAxis>(0), MotorState::Off);
            break;
        case Command::Configure: {
            const auto rmsCurrent = package.Read<float>(2);
            const auto gearRatio = package.Read<float>(3);
            const auto microSteps = package.Read<float>(4);
            stepperDriver.SetRmsCurrent(rmsCurrent);
            stepperDriver.SetGearRatio(gearRatio);
            stepperDriver.SetMicroSteps(microSteps);
            break;
        }
        case Command::Move: {
            const auto pitchAngle = package.Read<float>(0);
            const auto yawAngle = package.Read<float>(1);
            const auto angularSpeed = package.Read<float>(2);
            stepperDriver.MoveToTarget(pitchAngle, yawAngle, angularSpeed);
            break;
        }
        case Command::Advance: {
            const auto pitchAngle = package.Read<float>(0);
            const auto yawAngle = package.Read<float>(1);
            const auto angularSpeed = package.Read<float>(2);
            const auto currentPitch = stepperDriver.GetCurrentPitch();
            const auto currentYaw = stepperDriver.GetCurrentYaw();
            stepperDriver.MoveToTarget(currentPitch + pitchAngle, currentYaw + yawAngle, angularSpeed);
            break;
        }
        case Command::Origin:
            stepperDriver.MoveToTarget(0.0f, 0.0f, 0.5);
            break;
        case Command::Ack:
            break;
        default: {
            while (Serial.available() > 0) {
                volatile char clearSerial [[maybe_unused]] = Serial.read();
            }
            break;
        }
    };

    acknowledgePackage.Push(stepperDriver.GetCurrentPitch());
    acknowledgePackage.Push(stepperDriver.GetCurrentYaw());

    // Move Validation magic
    if (package.GetFlag() == Command::Move) {
        acknowledgePackage.Push<uint32_t>(69);
        acknowledgePackage.Push<uint32_t>(420);
    }

    Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);
}
