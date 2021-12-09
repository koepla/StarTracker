# StarTracker

Official repository for the diploma thesis of Ilja Koehler and Elias Plank

## Installing from source

### Prerequisites

First of all, you must have Visual Studio 2022 and vcpkg installed. I recommend to place vcpkg in a folder like C:/tools. You can download it via git:

```psh
git clone https://github.com/microsoft/vcpkg.git
```
After you installed vcpkg, you have to run the following commands in the vcpkg folder:

```psh
.\bootstrap-vcpkg.bat
```

```psh
.\vcpkg integrate install
```

```psh
.\vcpkg install glad:x64-windows-static
```
```psh
.\vcpkg install glfw3:x64-windows-static
```
```psh
.\vcpkg install glm:x64-windows-static
```
```psh
.\vcpkg install nlohmann-json:x64-windows-static
```
```psh
.\vcpkg install imgui[docking-experimental,opengl3-binding]:x64-windows-static
```

After running

```psh
.\vcpkg integrate install
```

once again, you should be good to go to build the project with Visual Studio.
