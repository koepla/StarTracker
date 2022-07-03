# StarTracker

Official repository for the diploma thesis of Ilja Koehler and Elias Plank

## Installing from source
It shall be noted that only x64 Windows is supported at the current time.

### Prerequisites

First of all, you must have **git**, **python3** and **cmake** installed. It is necessary to run the **Setup.py** file in the *scripts* folder. The script downloads **vcpkg** and installs the required dependencies. If one wants to maintain a local copy of **vcpkg**, it is not necessary to run the setup script. However in this case the *CMAKE_PREFIX_PATH* variable in *src/StarTracker/CMakeLists.txt* has to be adjusted accordingly.

### Building

After running the setup script, you should be good to go to build the desktop application with **cmake** . Open a terminal in the *src/StarTracker* folder, and run the following commands:

```psh
mkdir build
cd build
cmake ..
cmake --build .
```

This should place the executable and the required Assets folder into a folder called *Binaries*.