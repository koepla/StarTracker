#include "package.hpp"
#include "driver.hpp"

DriverConfig pitchLeft = {

    .microSteps = 256,
    .enablePin = 3,
    .stepPin = 2,
    .rxPin = 5,
    .txPin = 4
};

DriverConfig pitchRight = {

    .microSteps = 256,
    .enablePin = 7,
    .stepPin = 6,
    .rxPin = 9,
    .txPin = 8
};

DriverConfig yaw = {

    .microSteps = 256,
    .enablePin = 11,
    .stepPin = 10,
    .rxPin = 13,
    .txPin = 12
};

Driver driver(pitchLeft, pitchRight, yaw, 600 /* rms current */);
Protocol::Pack64 package;

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

            break;
        }
        case Protocol::Command::WAKEUP: {

            break;
        }
        case Protocol::Command::SLEEP: {

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

            break;
        }
        case Protocol::Command::ACK: {

            break;
        }
    };  
}
