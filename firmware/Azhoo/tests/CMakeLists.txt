cmake_minimum_required(VERSION 3.10)

set(SRCS
  src/tests_main.cpp
  src/picontroller_tests.cpp
  src/speedenvelope_tests.cpp
  src/azhoo_tests.cpp
  src/azhoocommands_tests.cpp
  mocks/mock_arduino.h
  mocks/mock_pwm.h
  mocks/mock_encoder.h
  ../Azhoo.cpp
  ../AzhooCommands.cpp
  )

add_executable(libromi_unit_tests_azhoo ${SRCS})

target_link_libraries(libromi_unit_tests_azhoo romiserial
                        gtest
                        gmock)

target_include_directories(libromi_unit_tests_azhoo
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

add_test(
        NAME libromi_unit_tests_azhoo
        COMMAND libromi_unit_tests_azhoo
        )

if (BUILD_COVERAGE)
        SETUP_TARGET_FOR_COVERAGE_LCOV(
                            NAME libromi_unit_tests_azhoo_coverage
                            EXECUTABLE ctest -V ${n_cores} -R "libromi"
                            #EXCLUDE "src/rcom-registry.cpp" "include/*" "examples/*"
                            DEPENDENCIES libromi_unit_tests_azhoo
                            )
endif()
