cmake_minimum_required(VERSION 3.20)
project(StarTracker)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_PREFIX_PATH "Vendor/vcpkg/installed/x64-windows-static/share/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_BINARY_DIR})

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_library(wininet wininet)
find_path(STB_INCLUDE_DIRS "../include/stb.h")

add_library(StarEngine
        StarEngine/DateTime.hpp
        StarEngine/DateTime.cpp
        StarEngine/Math.hpp
        StarEngine/Math.cpp
        StarEngine/StarEngine.hpp
        StarEngine/Ephemeris/CelestialBody.hpp
        StarEngine/Ephemeris/CelestialBody.cpp
        StarEngine/Ephemeris/FixedBody.hpp
        StarEngine/Ephemeris/FixedBody.cpp
        StarEngine/Ephemeris/KeplerianElements.hpp
        StarEngine/Ephemeris/KeplerianElements.cpp
        StarEngine/Ephemeris/SolarSystemBody.hpp
        StarEngine/Ephemeris/SolarSystemBody.cpp
        StarEngine/Ephemeris/Coordinates/Horizontal.hpp
        StarEngine/Ephemeris/Coordinates/Horizontal.cpp
        StarEngine/Ephemeris/Coordinates/Rectangular.hpp
        StarEngine/Ephemeris/Coordinates/Rectangular.cpp
        StarEngine/Ephemeris/Coordinates/Spherical.hpp
        StarEngine/Ephemeris/Coordinates/Spherical.cpp
        StarEngine/Ephemeris/Coordinates/Terrestrial.hpp
        StarEngine/Ephemeris/Coordinates/Terrestrial.cpp
        StarEngine/Ephemeris/Coordinates/Transform.hpp
        StarEngine/Ephemeris/Coordinates/Transform.cpp
        StarTracker/Core/ImageProcessing.cpp StarTracker/Core/ImageProcessing.hpp)

add_executable(StarTracker
        StarTracker/ExperimentalView.hpp
        StarTracker/ExperimentalView.cpp
        StarTracker/StarTrackerMain.cpp
        StarTracker/StarTrackerApplication.hpp
        StarTracker/StarTrackerApplication.cpp
        StarTracker/TrackableBodyView.hpp
        StarTracker/TrackableBodyView.cpp
        StarTracker/Core/Application.hpp
        StarTracker/Core/Application.cpp
        StarTracker/Core/Assert.hpp
        StarTracker/Core/Core.hpp
        StarTracker/Core/Input.hpp
        StarTracker/Core/Input.cpp
        StarTracker/Core/InputCode.hpp
        StarTracker/Core/InputCode.cpp
        StarTracker/Core/Tracker.hpp
        StarTracker/Core/Tracker.cpp
        StarTracker/Core/UIView.hpp
        StarTracker/Core/UIView.cpp
        StarTracker/Core/View.hpp
        StarTracker/Core/View.cpp
        StarTracker/Core/Window.hpp
        StarTracker/Core/Window.cpp
        StarTracker/Core/Events/Event.hpp
        StarTracker/Core/Events/Event.cpp
        StarTracker/Core/Events/EventDispatcher.hpp
        StarTracker/Core/Events/EventDispatcher.cpp
        StarTracker/Core/Events/KeyEvent.hpp
        StarTracker/Core/Events/KeyEvent.cpp
        StarTracker/Core/Events/MouseMoveEvent.hpp
        StarTracker/Core/Events/MouseMoveEvent.cpp
        StarTracker/Core/Events/WindowResizeEvent.hpp
        StarTracker/Core/Events/WindowResizeEvent.cpp
        StarTracker/Core/OpenGL/BufferLayout.hpp
        StarTracker/Core/OpenGL/BufferLayout.cpp
        StarTracker/Core/OpenGL/IndexBuffer.hpp
        StarTracker/Core/OpenGL/IndexBuffer.cpp
        StarTracker/Core/OpenGL/Shader.hpp
        StarTracker/Core/OpenGL/Shader.cpp
        StarTracker/Core/OpenGL/Texture.hpp
        StarTracker/Core/OpenGL/Texture.cpp
        StarTracker/Core/OpenGL/VertexArray.hpp
        StarTracker/Core/OpenGL/VertexArray.cpp
        StarTracker/Core/OpenGL/VertexBuffer.hpp
        StarTracker/Core/OpenGL/VertexBuffer.cpp
        StarTracker/Utils/File.hpp
        StarTracker/Utils/File.cpp
        StarTracker/Utils/GeoLocation.hpp
        StarTracker/Utils/GeoLocation.cpp
        StarTracker/Utils/HttpRequest.hpp
        StarTracker/Utils/HttpRequest.cpp
        StarTracker/Utils/Stopwatch.hpp
        StarTracker/Utils/Stopwatch.cpp
        StarTracker/Utils/Serial/Package.hpp
        StarTracker/Utils/Serial/Package.cpp
        StarTracker/Utils/Serial/Serial.hpp
        StarTracker/Utils/Serial/Serial.cpp
        StarTracker/Core/OpenGL/FrameBuffer.hpp
        StarTracker/Core/OpenGL/FrameBuffer.cpp
        StarTracker/StarTrackerView.hpp
        StarTracker/StarTrackerView.cpp
        StarTracker/Core/Events/MouseClickEvent.hpp
        StarTracker/Core/Events/MouseClickEvent.cpp
        StarTracker/Core/Events/MouseScrollEvent.hpp
        StarTracker/Core/Events/MouseScrollEvent.cpp
        StarTracker/Core/OpenGL/Camera.hpp
        StarTracker/Core/OpenGL/Camera.cpp
        StarTracker/Core/AssetDataBase.hpp
        StarTracker/Core/AssetDataBase.cpp
        StarTracker/Core/OpenGL/Renderer.cpp StarTracker/Core/OpenGL/Renderer.hpp StarTracker/ImageProcessingView.cpp StarTracker/ImageProcessingView.hpp)


target_include_directories(StarEngine PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(StarTracker PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(StarTracker PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(StarEngine PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(StarTracker PRIVATE StarEngine)
target_link_libraries(StarTracker PRIVATE glad::glad)
target_link_libraries(StarTracker PRIVATE glm::glm)
target_link_libraries(StarTracker PRIVATE glfw)
target_link_libraries(StarTracker PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(StarTracker PRIVATE imgui::imgui)
target_link_libraries(StarTracker PRIVATE wininet)

# Set the Asset Directories
set(ASSET_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets)
set(ASSET_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR}/Assets)

configure_file(${ASSET_SOURCE_DIR}/Ephemeris/CelestialBodies.json ${ASSET_TARGET_DIR}/Ephemeris/CelestialBodies.json COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSDisplay-Bold.ttf  ${ASSET_TARGET_DIR}/Fonts/SFNSDisplay-Bold.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSDisplay-Heavy.ttf ${ASSET_TARGET_DIR}/Fonts/SFNSDisplay-Heavy.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSDisplay-Light.ttf ${ASSET_TARGET_DIR}/Fonts/SFNSDisplay-Light.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSDisplay-Medium.ttf ${ASSET_TARGET_DIR}/Fonts/SFNSDisplay-Medium.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSDisplay-Regular.ttf ${ASSET_TARGET_DIR}/Fonts/SFNSDisplay-Regular.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSDisplay-Semibold.ttf ${ASSET_TARGET_DIR}/Fonts/SFNSDisplay-Semibold.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSDisplay-Thin.ttf  ${ASSET_TARGET_DIR}/Fonts/SFNSDisplay-Thin.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Fonts/SFNSText-RegularItalic.ttf ${ASSET_TARGET_DIR}/Fonts/SFNSText-RegularItalic.ttf COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/defaultFragment.glsl ${ASSET_TARGET_DIR}/Shaders/defaultFragment.glsl COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/defaultVertex.glsl ${ASSET_TARGET_DIR}/Shaders/defaultVertex.glsl COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/textureFragment.glsl ${ASSET_TARGET_DIR}/Shaders/textureFragment.glsl COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/textureVertex.glsl ${ASSET_TARGET_DIR}/Shaders/textureVertex.glsl COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/textureStackFragment.glsl ${ASSET_TARGET_DIR}/Shaders/textureStackFragment.glsl COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/textureStackVertex.glsl ${ASSET_TARGET_DIR}/Shaders/textureStackVertex.glsl COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Textures/pillarsOfCreation.jpg ${ASSET_TARGET_DIR}/Textures/pillarsOfCreation.jpg COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Textures/blue.png ${ASSET_TARGET_DIR}/Textures/blue.png COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Textures/yellow.png ${ASSET_TARGET_DIR}/Textures/yellow.png COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/kernelFragment.glsl ${ASSET_TARGET_DIR}/Shaders/kernelFragment.glsl COPYONLY)
configure_file(${ASSET_SOURCE_DIR}/Shaders/kernelVertex.glsl ${ASSET_TARGET_DIR}/Shaders/kernelVertex.glsl COPYONLY)