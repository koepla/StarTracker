#include "Package.hpp"
#include "Driver.hpp"

auto main() -> int {

    DriverConfig pitchLeftConf = {

        .MicroSteps = 256,
        .EnablePin = 3,
        .StepPin = 2,
        .RxPin = 5,
        .TxPin = 4,
        .RmsCurrent = 1000
    };

    DriverConfig pitchRightConf = {

        .MicroSteps = 256,
        .EnablePin = 7,
        .StepPin = 6,
        .RxPin = 9,
        .TxPin = 8,
        .RmsCurrent = 1000
    };

    DriverConfig yawConf = {

        .MicroSteps = 256,
        .EnablePin = 11,
        .StepPin = 10,
        .RxPin = 13,
        .TxPin = 12,
        .RmsCurrent = 600
    };

    Driver driver = Driver(pitchLeftConf, pitchRightConf, yawConf);
    Protocol::Pack32 package;

    init();
    driver.Init();
    Serial.begin(115200);

    while(true) {

        if(Serial.available() != sizeof(package)) {

            continue;
        }

        Serial.readBytes(reinterpret_cast<uint8_t*>(&package), sizeof(package));

        switch(package.Header.Flag) {

            case Protocol::Command::NONE: {

                package.Clear();
                
                break;
            }
            case Protocol::Command::WAKEUP: {

                MotorAxis axis = package.Read<MotorAxis>(0);
                driver.SetMotorState(axis, MotorState::ON);

                break;
            }
            case Protocol::Command::SLEEP: {

                MotorAxis axis = package.Read<MotorAxis>(0);
                driver.SetMotorState(axis, MotorState::OFF);

                break;
            }
            case Protocol::Command::CONF: {

                float pitch = package.Read<float>(0);
                float yaw = package.Read<float>(1);

                driver.SetCurrentPosition(pitch, yaw);

                break;
            }
            case Protocol::Command::MOVE: {

                float pitch = package.Read<float>(0);
                float yaw = package.Read<float>(1);

                driver.Move(pitch, yaw);

                break;
            }
            case Protocol::Command::ORIGIN: {

                driver.Move(0.0f, 0.0f);

                break;
            }
            case Protocol::Command::ACK: {

                // In this case the client expects the same package to be returned (ECHO)
                Serial.write(reinterpret_cast<uint8_t*>(&package), sizeof(package));

                break;
            }
        };     
    }

    return 0;
}