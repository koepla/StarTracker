#include "package.hpp"
#include "driver.hpp"

DriverConfig pitchLeftConf = {

    .microSteps = 256,
    .enablePin = 3,
    .stepPin = 2,
    .rxPin = 5,
    .txPin = 4,
    .rmsCurrent = 1000
};

DriverConfig pitchRightConf = {

    .microSteps = 256,
    .enablePin = 7,
    .stepPin = 6,
    .rxPin = 9,
    .txPin = 8,
    .rmsCurrent = 1000
};

DriverConfig yawConf = {

    .microSteps = 256,
    .enablePin = 11,
    .stepPin = 10,
    .rxPin = 13,
    .txPin = 12,
    .rmsCurrent = 600
};

Driver driver = Driver(pitchLeftConf, pitchRightConf, yawConf);
Protocol::Pack32 package;

void setup(){           

    driver.Init();
    Serial.begin(115200);
}

void loop(){

    if(Serial.available() != sizeof(package)) {

        return;
    }

    Serial.readBytes(reinterpret_cast<uint8_t*>(&package), sizeof(package));

    switch(package.header.flag) {

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
        case Protocol::Command::MOVE: {

            float pitch = package.Read<float>(0);
            float yaw = package.Read<float>(1);

            driver.Move(pitch, yaw);

            break;
        }
        case Protocol::Command::STOP: {

            break;
        }
        case Protocol::Command::ORIGIN: {

            driver.Move(0.0f, 0.0f);

            break;
        }
        case Protocol::Command::ACK: {

            // In this case the client expects the same package to be returned
            Serial.write(reinterpret_cast<uint8_t*>(&package), sizeof(package));

            break;
        }
    };  
}
