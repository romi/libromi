cmake_minimum_required(VERSION 3.10)

set(SRCS
    src/tests_main.cpp
    src/rover_tests.cpp
    src/cnc_range_tests.cpp)

add_executable(libromi_unit_tests ${SRCS})

target_link_libraries(  libromi_unit_tests
                        gtest
                        gmock
                        romi)

target_include_directories(libromi_unit_tests
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks
        ${CMAKE_CURRENT_SOURCE_DIR}/support)

add_test(
        NAME libromi_unit_tests
        COMMAND libromi_unit_tests
        )

if(BUILD_COVERAGE)
        SETUP_TARGET_FOR_COVERAGE_LCOV(
                            NAME libromi_unit_tests_coverage
                            EXECUTABLE ctest -V ${n_cores}
                            DEPENDENCIES libromi_unit_tests )
endif()
