import os
import subprocess
import platform
import errno

def panic(message):
        print(message)
        exit()

def self_path():
    return os.path.dirname(os.path.abspath(__file__))

def make_directory(path):
    try:
        os.makedirs(path)
    except (IOError, OSError) as exception:
        if exception.errno != errno.EEXIST:
            panic('Could not create folder!')

def change_directory(path):
    os.chdir(path)

# Change to root directory of the repository
change_directory(self_path())
change_directory('./../')
make_directory('./src/StarTracker/Vendor/')
change_directory('./src/StarTracker/Vendor/')

# Install vcpkg
print('Installing vcpkg...')
subprocess.call(["git", "clone", "https://github.com/microsoft/vcpkg.git"])
change_directory('./vcpkg/')
subprocess.call(["bootstrap-vcpkg.bat"])
subprocess.call(["vcpkg", "integrate", "install"])
subprocess.call(["vcpkg", "install", "glad:x64-windows-static"])
subprocess.call(["vcpkg", "install", "glfw3:x64-windows-static"])
subprocess.call(["vcpkg", "install", "glm:x64-windows-static"])
subprocess.call(["vcpkg", "install", "nlohmann-json:x64-windows-static"])
subprocess.call(["vcpkg", "install", "imgui[core,docking-experimental,opengl3-binding,glfw-binding]:x64-windows-static"])
subprocess.call(["vcpkg", "install", "stb:x64-windows-static"])
subprocess.call(["vcpkg", "integrate", "install"])