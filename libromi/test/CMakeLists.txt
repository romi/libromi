
cmake_minimum_required(VERSION 3.10)

set(SRCS_V
  src/tests_main.cpp
  src/tests_v.cpp
  )

add_executable(romi_rover_unit_tests_libromi_v ${SRCS_V})
target_link_libraries(romi_rover_unit_tests_libromi_v romi rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_v
    COMMAND romi_rover_unit_tests_libromi_v
)

##

set(SRCS_CNC
    src/tests_main.cpp
    src/tests_cncrange.cpp
    src/tests_cncadaptor.cpp
  )
add_executable(romi_rover_unit_tests_libromi_cnc ${SRCS_CNC})
target_link_libraries(romi_rover_unit_tests_libromi_cnc romi rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_cnc
    COMMAND romi_rover_unit_tests_libromi_cnc
)

##

set(SRCS_PATH
  src/tests_main.cpp
  src/tests_path.cpp
  )
add_executable(romi_rover_unit_tests_libromi_path ${SRCS_PATH})
target_link_libraries(romi_rover_unit_tests_libromi_path romi rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_path
    COMMAND romi_rover_unit_tests_libromi_path
)

##

set(SRCS_WHEELODOMETRY 
  src/tests_main.cpp
  src/tests_wheelodometry.cpp
  )

add_executable(romi_rover_unit_tests_libromi_wheelodometry ${SRCS_WHEELODOMETRY})
target_link_libraries(romi_rover_unit_tests_libromi_wheelodometry romi rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_wheelodometry
    COMMAND romi_rover_unit_tests_libromi_wheelodometry
)

##

set(SRCS_IMAGE_TESTS
    src/tests_main.cpp
    src/tests_image.cpp
    src/tests_imageio.cpp
  )

add_executable(romi_rover_unit_tests_libromi_image ${SRCS_IMAGE_TESTS})
target_link_libraries(romi_rover_unit_tests_libromi_image romi rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_image
    COMMAND romi_rover_unit_tests_libromi_image
)

##

set(SRCS_ROVER
  src/tests_main.cpp
  src/tests_roveroptions.cpp
  src/tests_roverscriptengine.cpp
  )
add_executable(romi_rover_unit_tests_libromi_rover ${SRCS_ROVER})
target_link_libraries(romi_rover_unit_tests_libromi_rover romi rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_rover
    COMMAND romi_rover_unit_tests_libromi_rover
)

##

set(SRCS_NAVIGATION
    src/tests_main.cpp
    src/tests_navigationsettings.cpp
    src/tests_navigationadaptor.cpp
    src/tests_remotenavigation.cpp
    src/tests_navigation.cpp
  )

add_executable(romi_rover_unit_tests_libromi_navigation ${SRCS_NAVIGATION})
target_link_libraries(romi_rover_unit_tests_libromi_navigation romi romi rpp rcompp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_navigation
    COMMAND romi_rover_unit_tests_libromi_navigation
)

##

set(SRCS_CRYSTALDISPLAY 
  src/tests_main.cpp
  src/tests_crystaldisplay.cpp
  )

add_executable(romi_rover_unit_tests_libromi_crystaldisplay ${SRCS_CRYSTALDISPLAY})
target_link_libraries(romi_rover_unit_tests_libromi_crystaldisplay rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_crystaldisplay
    COMMAND romi_rover_unit_tests_libromi_crystaldisplay
)

##

set(SRCS_JOYSTICK 
    src/tests_main.cpp
    src/tests_joystick.cpp
    src/tests_joystickinputdevice.cpp
  )

add_executable(romi_rover_unit_tests_libromi_joystick ${SRCS_JOYSTICK})
target_link_libraries(romi_rover_unit_tests_libromi_joystick rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_joystick
    COMMAND romi_rover_unit_tests_libromi_joystick
)

##

set(SRCS_SCRIPTLIST 
  src/tests_main.cpp
  src/tests_scriptlist.cpp
  )

add_executable(romi_rover_unit_tests_libromi_scriptlist ${SRCS_SCRIPTLIST})
target_link_libraries(romi_rover_unit_tests_libromi_scriptlist rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_scriptlist
    COMMAND romi_rover_unit_tests_libromi_scriptlist
)

##

set(SRCS_STATEMACHINE 
  src/tests_main.cpp
  src/tests_statemachine.cpp
  )

add_executable(romi_rover_unit_tests_libromi_statemachine ${SRCS_STATEMACHINE})
target_link_libraries(romi_rover_unit_tests_libromi_statemachine rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_statemachine
    COMMAND romi_rover_unit_tests_libromi_statemachine
)

##

set(SRCS_SPEEDTESTS
  src/tests_main.cpp
  src/tests_speedconverter.cpp
  src/tests_speedcontroller.cpp
  )

add_executable(romi_rover_unit_tests_libromi_speed ${SRCS_SPEEDTESTS})
target_link_libraries(romi_rover_unit_tests_libromi_speed rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_speed
    COMMAND romi_rover_unit_tests_libromi_speed
)

##

set(SRCS_SCRIPTMENU 
  src/tests_main.cpp
  src/tests_scriptmenu.cpp
  )

add_executable(romi_rover_unit_tests_libromi_scriptmenu ${SRCS_SCRIPTMENU})
target_link_libraries(romi_rover_unit_tests_libromi_scriptmenu rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_scriptmenu
    COMMAND romi_rover_unit_tests_libromi_scriptmenu
)

##

set(SRCS_STEPPERSETTINGS
  src/tests_main.cpp
  src/tests_steppersettings.cpp
  )
add_executable(romi_rover_unit_tests_libromi_steppersettings ${SRCS_STEPPERSETTINGS})
target_link_libraries(romi_rover_unit_tests_libromi_steppersettings rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_steppersettings
    COMMAND romi_rover_unit_tests_libromi_steppersettings
)

##

set(SRCS_OQUAM
    src/tests_main.cpp
    src/tests_oquam_section.cpp
    src/tests_oquam_print.cpp
    src/tests_oquam_script.cpp
    src/tests_oquam.cpp
  )
add_executable(romi_rover_unit_tests_libromi_oquam ${SRCS_OQUAM})
target_link_libraries(romi_rover_unit_tests_libromi_oquam rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_oquam
    COMMAND romi_rover_unit_tests_libromi_oquam
)

##
  
set(SRCS_BRUSHMOTORDRIVER 
  src/tests_main.cpp
  src/tests_brushmotordriver.cpp
  )

add_executable(romi_rover_unit_tests_libromi_brushmotordriver ${SRCS_BRUSHMOTORDRIVER})
target_link_libraries(romi_rover_unit_tests_libromi_brushmotordriver romi romiserial rpp rcompp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_brushmotordriver
    COMMAND romi_rover_unit_tests_libromi_brushmotordriver
)

##
  
if(BUILD_COVERAGE)
    SETUP_TARGET_FOR_COVERAGE_LCOV(
            NAME romi_rover_libromi_coverage
            EXECUTABLE ctest -V ${n_cores} -R "romi_rover"
            DEPENDENCIES    romi_rover_unit_tests_libromi_v
                            romi_rover_unit_tests_libromi_cnc
                            romi_rover_unit_tests_libromi_path
                            romi_rover_unit_tests_libromi_navigationsettings
                            romi_rover_unit_tests_libromi_wheelodometry
                            romi_rover_unit_tests_libromi_image
                            romi_rover_unit_tests_libromi_rover
                            romi_rover_unit_tests_libromi_navigation
                            romi_rover_unit_tests_libromi_crystaldisplay
                            romi_rover_unit_tests_libromi_joystick
                            romi_rover_unit_tests_libromi_scriptlist
                            romi_rover_unit_tests_libromi_statemachine
                            romi_rover_unit_tests_libromi_speed
                            romi_rover_unit_tests_libromi_scriptmenu
                            romi_rover_unit_tests_libromi_steppersettings
                            romi_rover_unit_tests_libromi_oquam
                            romi_rover_unit_tests_libromi_brushmotordriver
    )
endif()

