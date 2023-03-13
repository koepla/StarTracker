cmake_minimum_required(VERSION 3.22)

# Set the project name and language
project(StarTracker VERSION 0.0.1 DESCRIPTION "StarTracker" LANGUAGES CXX C)
include(CTest)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add subprojects
add_subdirectory(submodules/fmt)
add_subdirectory(submodules/glfw)
add_subdirectory(submodules/glm)
add_subdirectory(submodules/json)
add_subdirectory(submodules/googletest)
add_subdirectory(extern)
add_subdirectory(libengine)
add_subdirectory(libtracker)
add_subdirectory(utility)

file(GLOB STARTRACKER_SOURCE_LIST 
	${CMAKE_CURRENT_LIST_DIR}/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/*.hpp)

add_executable(startracker ${STARTRACKER_SOURCE_LIST})
target_include_directories(startracker PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(startracker PUBLIC "libtracker" "libengine" "utility")

# Copy Assets to the Output Directory
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_INSTALL_PREFIX})

enable_testing()
add_subdirectory(tests)

