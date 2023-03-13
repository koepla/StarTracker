# StarTracker

This repository is part of the diploma thesis of `Ilja Roman Köhler` and `Elias Engelbert Plank`. In short, the goal is to provide a software and hardware solution for the following question: Which technologies and algorithms are necessary to automatically track a range of different celestial bodies with an off the shelf camera?

---

## Contributors

[`Ilja Roman Köhler`](https://github.com/KoehIl180151 "Hardware part")

Responsible for the hardware, which involves designing and putting together a circuit that uses the almighty `ATMega-328p` chip as well as `TMC2209 drivers`, design and construction of the 3D-printed parts of the tracking mount, and firmware development for the circuit.

[`Elias Engelbert Plank`](https://github.com/PlanEl180223 "Software part")

Responsible for most of the software, which involves the ephemeris engine and the actual client application.

---

## How does StarTracker work?

To achieve the above goal, a construction made with the help of FDM printing, a circuit board, and a specially developed software are used. Specifically, an off the shelf camera mounted on the construction can be moved to any point in the sky by using stepper motors. By calculating the position of any celestial body in the client application, one can point the camera at basically anything that has spherical coordinates.

### Construction

The FDM parts were designed in a way so that they can be easily procued with any 3D printer (even entry-level devices) and still have the necessary stability. We recommend using PETG over PLA filament due to its superior material properties. The entire 3D design process is done in `Autodesk Fusion 360`, since Autodesk offers free licences for students, as well as a tool for designing circuit boards (`Autodesk Eagle`).

### Electronics and circuit

The camera is steered by `Nema 17 stepper motors`, which offer the perfect balance of power and size. The motors are driven by `TMC-2209 drivers` due to their reliability and robustness. The heart of the circuit is a `ATMega-328p` microcontroller, currently in the form of an Arduino Nano. However, there are plans to replace the Arduino Nano with just the ATMega chip itself.

### Client application

The client application, which is developed as a graphical desktop application entirely in C++ and GLSL, has a list of celestial bodies whose position can be computed. This list can be extended by the user. The position of each celestial body in the sky is determined in real time using our own ephemeris engine. When the user selects a target object, the position data is transmitted via a two-way protocol to the microcontroller on the circuit, whereupon the microcontroller sets the stepper motors to the correct position.

---

## Building

First of all, you must have git and CMake installed. As we use presets for our CMake configuration, you need a relatively recent version that supports presets. It is extremely important, that the repository is clonsed recursively, due to the use of submodules.

### Windows

On Windows, it is recommended to use Visual Studio 2022 together with the CMake extension. CMake will grab the dependencies upon configure, therefore no libraries have to be manually installed. You can then open the src/libtracker folder with Visual Studio that contains the CMakeLists.txt file. After CMake will finish with the configuration, you can choose a preset and build the application.

### Linux

On Linux, it is recommended to use JetBrains CLion, as it integrates with CMake. If you already have `curl` and `libcurl4-openssl-dev` installed, the CMake configuration will use those. Otherwise, curl will be automatically fetched, therefore no install is required. You can then open the src/libtracker folder with CLion. After CMake will finish with the configuration, you can choose a preset and build the application.

Here's a folder structure to feel secure and find your way around:

```
[root repo folder]
  |
  + [doc] // containing detailed project documentation
  |   |
  |   + [progress_reports] // containing status reports
  |                        // additionally github projects is used for documentation 
  |
  + [hwr] // containg schematics of the hardware parts
  |
  + [src] // containing the actual source code
      |
      + [tracker] // containing the code of the client application
      |   |
      |   + [libengine] // holds the source code of the ephemeris engine, builds to static library
      |   |
      |   + [libtracker] // the actual client application, links with libengine
      |
      + [firmware] // containing the firmware for the circuit
```

---
