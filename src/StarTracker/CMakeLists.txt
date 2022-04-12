cmake_minimum_required(VERSION 3.20)
project(StarTracker)

if(MSVC)
	add_compile_options(
		$<$<CONFIG:>:/MT> #---------|
		$<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
		$<$<CONFIG:Release>:/MT> #--|
		$<$<CONFIG:RelWithDebInfo>:/MT>
	)
endif()

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
find_package(tinyobjloader CONFIG REQUIRED)
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
		StarEngine/Ephemeris/Coordinates/Transform.cpp)

add_executable(StarTracker
		StarTracker/ExperimentalView.hpp
		StarTracker/ExperimentalView.cpp
		StarTracker/StarTrackerMain.cpp
		StarTracker/StarTrackerApplication.hpp
		StarTracker/StarTrackerApplication.cpp
		StarTracker/TrackableBodyView.hpp
		StarTracker/TrackableBodyView.cpp
		StarTracker/Core/Allocator.hpp
		StarTracker/Core/Allocator.cpp
		StarTracker/Core/Application.hpp
		StarTracker/Core/Application.cpp
		StarTracker/Core/Assert.hpp
		StarTracker/Core/Core.hpp
		StarTracker/Core/Core.cpp
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
		StarTracker/Core/OpenGL/Renderer.hpp 
		StarTracker/Core/OpenGL/Renderer.cpp 
		StarTracker/Core/ImageProcessing.hpp
		StarTracker/Core/ImageProcessing.cpp
		StarTracker/ImageProcessingView.hpp
		StarTracker/ImageProcessingView.cpp
		StarTracker/Core/OpenGL/Model.hpp
		StarTracker/Core/OpenGL/Model.cpp
		StarTracker/Core/Logger.hpp
		StarTracker/Core/Logger.cpp
		StarTracker/Core/BodyLibrary.hpp
		StarTracker/Core/BodyLibrary.cpp
		StarTracker/UI/UI.hpp 
		StarTracker/UI/UI.cpp)

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
target_link_libraries(StarTracker PRIVATE tinyobjloader::tinyobjloader)

# Set the Binary Output Directory
set_property(TARGET StarTracker PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set_property(TARGET StarTracker PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set_property(TARGET StarTracker PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set_property(TARGET StarTracker PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set_property(TARGET StarTracker PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)

# Copy Assets to the Output Directory
set(ASSET_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/StarTracker/Assets)
set(ASSET_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
file(COPY ${ASSET_SOURCE_DIR} DESTINATION ${ASSET_TARGET_DIR})