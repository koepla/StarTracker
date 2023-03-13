# utility library

file(GLOB UTILITY_SOURCE_LIST
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

add_library(utility ${UTILITY_SOURCE_LIST})
target_include_directories(utility PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(utility PRIVATE "fmt")