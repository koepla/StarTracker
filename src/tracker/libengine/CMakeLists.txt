# libengine cmake configuration

file(GLOB LIBENGINE_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/ephemeris/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ephemeris/*.hpp)

add_library(libengine ${LIBENGINE_SOURCE_LIST})
target_include_directories(libengine PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libengine PRIVATE "nlohmann_json" "fmt" "utility")