# libtracker cmake configuration

file(GLOB LIBTRACKER_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/arch/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/arch/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/arch/${TARGET_BUILD_PLATFORM}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/arch/${TARGET_BUILD_PLATFORM}/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/core/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/core/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/graphics/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/graphics/*.hpp)

add_library(libtracker ${LIBTRACKER_SOURCE_LIST})
target_include_directories(libtracker PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

if(${TARGET_BUILD_PLATFORM} STREQUAL "win32")
	target_link_libraries(libtracker PUBLIC "wininet")
elseif(${TARGET_BUILD_PLATFORM} STREQUAL "linux")
	 find_package(CURL REQUIRED)
	 if (CURL_FOUND)
		message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
		message(STATUS "Using CURL include dir(s): ${CURL_INCLUDE_DIRS}")
		message(STATUS "Using CURL lib(s): ${CURL_LIBRARIES}")
	 else()
		message(STATUS "Fetching CURL")
		include(FetchContent)
		FetchContent_Declare(CURL GIT_REPOSITORY https://github.com/curl/curl.git)
		FetchContent_MakeAvailable(CURL)
	 endif()
	 target_link_libraries(libtracker PUBLIC ${CURL_LIBRARIES})
	 target_include_directories(libtracker PUBLIC ${CURL_INCLUDE_DIRS})
endif()

target_link_libraries(libtracker PUBLIC
	"fmt"
	"glad"
	"glm"
	"glfw"
	"ImGui"
	"ImPlot"
	"libengine"
	"nlohmann_json"
	)