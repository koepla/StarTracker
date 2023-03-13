# tests cmake configuration

set(TEST_TARGET_LIST "test-libengine" "test-libtracker")

foreach(TEST_TARGET ${TEST_TARGET_LIST})
    add_executable(${TEST_TARGET} "${TEST_TARGET}.cpp")
    target_link_libraries(${TEST_TARGET} PUBLIC "libengine" "libtracker" "fmt" "GTest::gtest" "GTest::gtest_main" "GTest::gmock" "GTest::gmock_main")
    target_include_directories(${TEST_TARGET} PUBLIC ${CMAKE_SOURCE_DIR})
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endforeach()
