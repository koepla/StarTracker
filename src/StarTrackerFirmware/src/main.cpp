#define SERIAL_TX_BUFFER_SIZE 256
#define SERIAL_RX_BUFFER_SIZE 256

#include "Package.hpp"
#include "Driver.hpp"

void blink() {

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}

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

    Driver driver{ pitchLeftConf, pitchRightConf, yawConf };
    Protocol::Pack32 package{};
    Protocol::Pack32 acknowledgePackage{ Protocol::Command::ACK };

    init();
    driver.Init();
    Serial.begin(115200);

    while(true) {

        blink();

        if(Serial.available() != sizeof(package)) {

            continue;
        }

        Serial.readBytes(reinterpret_cast<uint8_t*>(&package), sizeof package);

        switch(package.GetFlag()) {

            case Protocol::Command::NONE: {

                package.Clear();
                
                break;
            }
            case Protocol::Command::WAKEUP: {

                MotorAxis axis = package.Read<MotorAxis>(0);
                driver.SetMotorState(axis, MotorState::ON);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case Protocol::Command::SLEEP: {

                MotorAxis axis = package.Read<MotorAxis>(0);
                driver.SetMotorState(axis, MotorState::OFF);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case Protocol::Command::CONF: {

                float pitch = package.Read<float>(0);
                float yaw = package.Read<float>(1);
                driver.SetCurrentPosition(pitch, yaw);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case Protocol::Command::MOVE: {

                float pitch = package.Read<float>(0);
                float yaw = package.Read<float>(1);
                driver.Move(pitch, yaw);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case Protocol::Command::ORIGIN: {

                driver.Move(0.0f, 0.0f);
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            case Protocol::Command::ACK: {

                // In this case the client only expects an ack package
                Serial.write(reinterpret_cast<uint8_t*>(&acknowledgePackage), sizeof acknowledgePackage);

                break;
            }
            default: {

                while(Serial.available() > 0) {

                    char clearSerial [[maybe_unused]] = Serial.read();
                }
            }
        };  
    }

    return 0;
}