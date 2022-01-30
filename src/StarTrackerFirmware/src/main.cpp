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
    Serial.begin(115200);

    StarTracker::StepperDriver stepperDriver{ leftPitchConfig, rightPitchConfig, yawConfig };
    StarTracker::Pack32 package{};
    StarTracker::Pack32 acknowledgePackage{ StarTracker::Command::ACK };

    while(true) {

        if(Serial.available() < static_cast<int>(sizeof package)) {

            continue;
        }

        Serial.readBytes(reinterpret_cast<uint8_t*>(&package), sizeof package);

        switch(package.GetFlag()) {

            case StarTracker::Command::NONE: {

                package.Clear();
                
                break;
            }
            case StarTracker::Command::WAKEUP: {

                stepperDriver.SetMotorState(package.Read<StarTracker::MotorAxis>(0), StarTracker::MotorState::ON);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case StarTracker::Command::SLEEP: {

                stepperDriver.SetMotorState(package.Read<StarTracker::MotorAxis>(0), StarTracker::MotorState::OFF);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case StarTracker::Command::CONF: {

                const auto pitchAngle = package.Read<float>(0);
                const auto yawAngle = package.Read<float>(1);
                stepperDriver.SetCurrentPosition(pitchAngle, yawAngle);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case StarTracker::Command::MOVE: {

                const auto pitchAngle = package.Read<float>(0);
                const auto yawAngle = package.Read<float>(1);
                stepperDriver.MoveToTarget(pitchAngle, yawAngle, 5000);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case StarTracker::Command::ORIGIN: {

                stepperDriver.MoveToTarget(0.0f, 0.0f, 5000);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case StarTracker::Command::ACK: {

                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            default: {

                while(Serial.available() > 0) {

                    char clearSerial [[maybe_unused]] = Serial.read();
                }
                break;
            }
        };  

        package.Clear();
    }

    return 0;
}