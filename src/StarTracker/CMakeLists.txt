cmake_minimum_required(VERSION 3.21)
project(StarTracker)

set(CMAKE_CXX_STANDARD 23)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_library(wininet wininet)
find_path(STB_INCLUDE_DIRS "stb.h")

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
        )

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
        )


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

configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Ephemeris/CelestialBodies.json ${CMAKE_CURRENT_BINARY_DIR}/Assets/Ephemeris/CelestialBodies.json COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSDisplay-Bold.ttf  ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSDisplay-Bold.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSDisplay-Heavy.ttf ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSDisplay-Heavy.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSDisplay-Light.ttf ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSDisplay-Light.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSDisplay-Medium.ttf ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSDisplay-Medium.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSDisplay-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSDisplay-Regular.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSDisplay-Semibold.ttf ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSDisplay-Semibold.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSDisplay-Thin.ttf  ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSDisplay-Thin.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Fonts/SFNSText-RegularItalic.ttf ${CMAKE_CURRENT_BINARY_DIR}/Assets/Fonts/SFNSText-RegularItalic.ttf COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Shaders/defaultFragment.glsl ${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/defaultFragment.glsl COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Shaders/defaultVertex.glsl ${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/defaultVertex.glsl COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Shaders/textureFragment.glsl ${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/textureFragment.glsl COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Shaders/textureVertex.glsl ${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/textureVertex.glsl COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets/Textures/pillarsOfCreation.jpg ${CMAKE_CURRENT_BINARY_DIR}/Assets/Textures/pillarsOfCreation.jpg COPYONLY)
