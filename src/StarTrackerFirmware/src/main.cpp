#include "Package.hpp"
#include "StepperDriver.hpp"

auto main() -> int {

    StarTracker::StepperDriverConfig leftPitchConfig{};
    leftPitchConfig.Microsteps = 256;
    leftPitchConfig.EnablePin = 3;
    leftPitchConfig.StepPin = 2;
    leftPitchConfig.RxPin = 5;
    leftPitchConfig.TxPin = 4;
    leftPitchConfig.RmsCurrent = 600;

    StarTracker::StepperDriverConfig rightPitchConfig{};
    rightPitchConfig.Microsteps = 256;
    rightPitchConfig.EnablePin = 7;
    rightPitchConfig.StepPin = 6;
    rightPitchConfig.RxPin = 9;
    rightPitchConfig.TxPin = 8;
    rightPitchConfig.RmsCurrent = 600;

    StarTracker::StepperDriverConfig yawConfig{};
    yawConfig.Microsteps = 256;
    yawConfig.EnablePin = 11;
    yawConfig.StepPin = 10;
    yawConfig.RxPin = 13;
    yawConfig.TxPin = 12;
    yawConfig.RmsCurrent = 600;

    init();
    Serial.begin(115200, SERIAL_8N1);

    StarTracker::StepperDriver stepperDriver{ leftPitchConfig, rightPitchConfig, yawConfig };
    StarTracker::Pack32 package{};
    StarTracker::Pack32 acknowledgePackage{ StarTracker::Command::ACK };

    while(true) {

        package.Clear();

        if(static_cast<size_t>(Serial.available()) < sizeof(package)) {

            continue;
        }

        Serial.readBytes(reinterpret_cast<uint8_t*>(&package), sizeof(package));

        switch(package.GetFlag()) {

            case StarTracker::Command::NONE: {
                
                break;
            }
            case StarTracker::Command::WAKEUP: {

                stepperDriver.SetMotorState(package.Read<StarTracker::MotorAxis>(0), StarTracker::MotorState::ON);

                break;
            }
            case StarTracker::Command::SLEEP: {

                stepperDriver.SetMotorState(package.Read<StarTracker::MotorAxis>(0), StarTracker::MotorState::OFF);

                break;
            }
            case StarTracker::Command::CONF: {

                const auto pitchAngle = package.Read<float>(0);
                const auto yawAngle = package.Read<float>(1);
                stepperDriver.SetCurrentPosition(pitchAngle, yawAngle);

                break;
            }
            case StarTracker::Command::MOVE: {

                const auto pitchAngle = package.Read<float>(0);
                const auto yawAngle = package.Read<float>(1);
                stepperDriver.MoveToTarget(pitchAngle, yawAngle, 5000);

                break;
            }
            case StarTracker::Command::ORIGIN: {

                stepperDriver.MoveToTarget(0.0f, 0.0f, 5000);

                break;
            }
            case StarTracker::Command::ACK: {


                break;
            }
            default: {

                while(Serial.available() > 0) {

                    volatile char clearSerial [[maybe_unused]] = Serial.read();
                }
                break;
            }
        };  

        Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof(acknowledgePackage));
    }

    return 0;
}