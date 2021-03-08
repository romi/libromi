
cmake_minimum_required(VERSION 3.10)

# TBD: This should really link with the libromi library.
# Remove this include when warnings are complete.
MESSAGE("Libromi CXX Flags = ${CMAKE_CXX_FLAGS}")
MESSAGE("Libromi C Flags = ${CMAKE_C_FLAGS}")

include_directories( "../include")

set(SRCS_V
  src/tests_main.cpp
  src/tests_v.cpp
  ../include/v.h
  ../src/v.cpp
  )

add_executable(romi_rover_unit_tests_libromi_v ${SRCS_V})
target_link_libraries(romi_rover_unit_tests_libromi_v rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_v
    COMMAND romi_rover_unit_tests_libromi_v
)

##

set(SRCS_CNCRANGE
  src/tests_main.cpp
  src/tests_cncrange.cpp
  ../include/api/CNCRange.h
  ../include/v.h
  ../src/api/CNCRange.cpp
  ../src/v.cpp
  )
add_executable(romi_rover_unit_tests_libromi_cncrange ${SRCS_CNCRANGE})
target_link_libraries(romi_rover_unit_tests_libromi_cncrange rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_cncrange
    COMMAND romi_rover_unit_tests_libromi_cncrange
)

##

set(SRCS_PATH
  src/tests_main.cpp
  src/tests_path.cpp
  ../include/api/CNCRange.h
  ../include/api/Path.h
  ../include/v.h
  ../src/api/CNCRange.cpp
  ../src/api/Path.cpp
  ../src/v.cpp
  )
add_executable(romi_rover_unit_tests_libromi_path ${SRCS_PATH})
target_link_libraries(romi_rover_unit_tests_libromi_path rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_path
    COMMAND romi_rover_unit_tests_libromi_path
)

##

set(SRCS_NAVIGATIONSETTINGS 
  src/tests_main.cpp
  src/tests_navigationsettings.cpp
  ../include/NavigationSettings.h
  )

add_executable(romi_rover_unit_tests_libromi_navigationsettings ${SRCS_NAVIGATIONSETTINGS})
include_directories( "../src" )
target_link_libraries(romi_rover_unit_tests_libromi_navigationsettings rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_navigationsettings
    COMMAND romi_rover_unit_tests_libromi_navigationsettings
)


##

set(SRCS_WHEELODOMETRY 
  src/tests_main.cpp
  src/tests_wheelodometry.cpp
  ../include/WheelOdometry.h
  ../src/WheelOdometry.cpp
  )

add_executable(romi_rover_unit_tests_libromi_wheelodometry ${SRCS_WHEELODOMETRY})
include_directories( "../src" )
target_link_libraries(romi_rover_unit_tests_libromi_wheelodometry rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_wheelodometry
    COMMAND romi_rover_unit_tests_libromi_wheelodometry
)

##

set(SRCS_IMAGE
  src/tests_main.cpp
  src/tests_image.cpp
  ../include/Image.h
  ../src/Image.cpp
  )

add_executable(romi_rover_unit_tests_libromi_image ${SRCS_IMAGE})
target_link_libraries(romi_rover_unit_tests_libromi_image rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_image
    COMMAND romi_rover_unit_tests_libromi_image
)

##

set(SRCS_IMAGEIO
  src/tests_main.cpp
  src/tests_imageio.cpp
  ../include/Image.h
  ../include/ImageIO.h
  ../src/Image.cpp
  ../src/ImageIO.cpp
  )
add_executable(romi_rover_unit_tests_libromi_imageio ${SRCS_IMAGEIO})
target_link_libraries(romi_rover_unit_tests_libromi_imageio rpp gtest gmock jpeg png)

add_test(
    NAME romi_rover_unit_tests_libromi_imageio
    COMMAND romi_rover_unit_tests_libromi_imageio
)

##

set(SRCS_ROVEROPTIONS
  src/tests_main.cpp
  src/tests_roveroptions.cpp
  ../include/GetOpt.h
  ../include/rover/RoverOptions.h
  ../src/GetOpt.cpp
  ../src/rover/RoverOptions.cpp
  )
add_executable(romi_rover_unit_tests_libromi_roveroptions ${SRCS_ROVEROPTIONS})
target_link_libraries(romi_rover_unit_tests_libromi_roveroptions rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_roveroptions
    COMMAND romi_rover_unit_tests_libromi_roveroptions
)

##

set(SRCS_NAVIGATIONADAPTOR 
  src/tests_main.cpp
  src/tests_navigationadaptor.cpp
  mock/mock_navigation.h
  ../include/rpc/NavigationAdaptor.h
  ../src/rpc/NavigationAdaptor.cpp
  )

add_executable(romi_rover_unit_tests_libromi_navigationadaptor ${SRCS_NAVIGATIONADAPTOR})
include_directories( "../src" )
target_link_libraries(romi_rover_unit_tests_libromi_navigationadaptor romi rpp rcompp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_navigationadaptor
    COMMAND romi_rover_unit_tests_libromi_navigationadaptor
)

##

set(SRCS_DEFAULTNAVIGATION 
  src/tests_main.cpp
  src/tests_defaultnavigation.cpp
  mock/mock_motordriver.h
        ../include/Navigation.h
        ../src/Navigation.cpp
  )

add_executable(romi_rover_unit_tests_libromi_defaultnavigation ${SRCS_DEFAULTNAVIGATION})
include_directories( "../src" )
target_link_libraries(romi_rover_unit_tests_libromi_defaultnavigation romi rpp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_defaultnavigation
    COMMAND romi_rover_unit_tests_libromi_defaultnavigation
)

##

set(SRCS_REMOTENAVIGATION 
  src/tests_main.cpp
  src/tests_remotenavigation.cpp
  ../include/rpc/RemoteNavigation.h
  ../src/rpc/RemoteNavigation.cpp
  )

add_executable(romi_rover_unit_tests_libromi_remotenavigation ${SRCS_REMOTENAVIGATION})
target_link_libraries(romi_rover_unit_tests_libromi_remotenavigation rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_remotenavigation
    COMMAND romi_rover_unit_tests_libromi_remotenavigation
)

##

set(SRCS_CRYSTALDISPLAY 
  src/tests_main.cpp
  src/tests_crystaldisplay.cpp
  mock/mock_romiserialclient.h
  ../include/CrystalDisplay.h
  ../src/ui/CrystalDisplay.cpp
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
  mock/Linux-Mock.cpp
  ../include/api/Joystick.h
  ../include/LinuxJoystick.h
  ../src/ui/LinuxJoystick.cpp
  )

add_executable(romi_rover_unit_tests_libromi_joystick ${SRCS_JOYSTICK})
target_link_libraries(romi_rover_unit_tests_libromi_joystick rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_joystick
    COMMAND romi_rover_unit_tests_libromi_joystick
)

##

set(SRCS_JOYSTICKINPUTDEVICE 
  src/tests_main.cpp
  src/tests_joystickinputdevice.cpp
  mock/mock_eventmapper.h
  mock/mock_joystick.h
  ../include/JoystickInputDevice.h
  ../src/ui/JoystickInputDevice.cpp
  )

add_executable(romi_rover_unit_tests_libromi_joystickinputdevice ${SRCS_JOYSTICKINPUTDEVICE})
target_link_libraries(romi_rover_unit_tests_libromi_joystickinputdevice rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_joystickinputdevice
    COMMAND romi_rover_unit_tests_libromi_joystickinputdevice
)

##

set(SRCS_SCRIPTLIST 
  src/tests_main.cpp
  src/tests_scriptlist.cpp
  mock/mock_inputdevice.h
  mock/mock_navigation.h
  mock/mock_display.h
  ../include/ScriptList.h
  ../src/ScriptList.cpp
  )

add_executable(romi_rover_unit_tests_libromi_scriptlist ${SRCS_SCRIPTLIST})
target_link_libraries(romi_rover_unit_tests_libromi_scriptlist rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_scriptlist
    COMMAND romi_rover_unit_tests_libromi_scriptlist
)

##

set(SRCS_ROVERSCRIPTENGINE 
  src/tests_main.cpp
  src/tests_roverscriptengine.cpp
  mock/mock_inputdevice.h
  mock/mock_navigation.h
  mock/mock_display.h
  ../include/rover/Rover.h
  ../include/rover/RoverScriptEngine.h
  ../src/rover/Rover.cpp
  ../src/rover/RoverScriptEngine.cpp
  )

add_executable(romi_rover_unit_tests_libromi_roverscriptengine ${SRCS_ROVERSCRIPTENGINE})
target_link_libraries(romi_rover_unit_tests_libromi_roverscriptengine rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_roverscriptengine
    COMMAND romi_rover_unit_tests_libromi_roverscriptengine
)

##

set(SRCS_STATEMACHINE 
  src/tests_main.cpp
  src/tests_statemachine.cpp
  ../include/api/StateMachine.h
  )

add_executable(romi_rover_unit_tests_libromi_statemachine ${SRCS_STATEMACHINE})
target_link_libraries(romi_rover_unit_tests_libromi_statemachine rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_statemachine
    COMMAND romi_rover_unit_tests_libromi_statemachine
)

##

set(SRCS_SPEEDCONVERTER 
  src/tests_main.cpp
  src/tests_speedconverter.cpp
  mock/mock_navigation.h
  ../include/SpeedConverter.h
  ../src/SpeedConverter.cpp
  )

add_executable(romi_rover_unit_tests_libromi_speedconverter ${SRCS_SPEEDCONVERTER})
target_link_libraries(romi_rover_unit_tests_libromi_speedconverter rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_speedconverter
    COMMAND romi_rover_unit_tests_libromi_speedconverter
)

##

set(SRCS_SPEEDCONTROLLER 
  src/tests_main.cpp
  src/tests_speedcontroller.cpp
  mock/mock_navigation.h
        ../include/SpeedController.h
        ../src/SpeedController.cpp
  ../include/SpeedConverter.h
  ../src/SpeedConverter.cpp
  )

add_executable(romi_rover_unit_tests_libromi_speedcontroller ${SRCS_SPEEDCONTROLLER})
target_link_libraries(romi_rover_unit_tests_libromi_speedcontroller rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_speedcontroller
    COMMAND romi_rover_unit_tests_libromi_speedcontroller
)

##

set(SRCS_SCRIPTMENU 
  src/tests_main.cpp
  src/tests_scriptmenu.cpp
  mock/mock_inputdevice.h
  mock/mock_navigation.h
  mock/mock_display.h
  ../include/ScriptMenu.h
  ../src/ScriptMenu.cpp
  )

add_executable(romi_rover_unit_tests_libromi_scriptmenu ${SRCS_SCRIPTMENU})
target_link_libraries(romi_rover_unit_tests_libromi_scriptmenu rcompp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_scriptmenu
    COMMAND romi_rover_unit_tests_libromi_scriptmenu
)

##

set(SRCS_CNCADAPTOR
  src/tests_main.cpp
  src/tests_cncadaptor.cpp
  ../include/rpc/CNCAdaptor.h
  ../src/rpc/CNCAdaptor.cpp
  )
add_executable(romi_rover_unit_tests_libromi_cncadaptor ${SRCS_CNCADAPTOR})
target_link_libraries(romi_rover_unit_tests_libromi_cncadaptor rpp romi rcompp gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_cncadaptor
    COMMAND romi_rover_unit_tests_libromi_cncadaptor
)

##

set(SRCS_STEPPERSETTINGS
  src/tests_main.cpp
  src/tests_steppersettings.cpp
  ../include/oquam/StepperSettings.h
  ../src/oquam/StepperSettings.cpp
  )
add_executable(romi_rover_unit_tests_libromi_steppersettings ${SRCS_STEPPERSETTINGS})
target_link_libraries(romi_rover_unit_tests_libromi_steppersettings rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_steppersettings
    COMMAND romi_rover_unit_tests_libromi_steppersettings
)

##

set(SRCS_SECTION
  src/tests_main.cpp
  src/tests_oquam_section.cpp
  ../include/oquam/Section.h
  ../src/oquam/Section.cpp
  ../include/oquam/SmoothPath.h
  ../src/oquam/SmoothPath.cpp
  ../include/oquam/ATDC.h
  ../src/oquam/ATDC.cpp
  ../src/oquam/is_valid.cpp
  ../include/oquam/is_valid.h
  ../include/oquam/print.h
  ../src/oquam/print.cpp
  )
add_executable(romi_rover_unit_tests_libromi_oquam_section ${SRCS_SECTION})
target_link_libraries(romi_rover_unit_tests_libromi_oquam_section rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_oquam_section
    COMMAND romi_rover_unit_tests_libromi_oquam_section
)

##

set(SRCS_PRINT
  src/tests_main.cpp
  src/tests_oquam_print.cpp
  ../include/oquam/SmoothPath.h
  ../src/oquam/SmoothPath.cpp
  ../include/oquam/ATDC.h
  ../src/oquam/ATDC.cpp
  ../include/oquam/Section.h
  ../src/oquam/Section.cpp
  ../include/oquam/print.h
  ../src/oquam/print.cpp
  ../src/oquam/is_valid.cpp
  ../include/oquam/is_valid.h
  )
add_executable(romi_rover_unit_tests_libromi_oquam_print ${SRCS_PRINT})
target_link_libraries(romi_rover_unit_tests_libromi_oquam_print rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_oquam_print
    COMMAND romi_rover_unit_tests_libromi_oquam_print
)

##

set(SRCS_SCRIPT
  src/tests_main.cpp
  src/tests_oquam_script.cpp
  ../include/oquam/SmoothPath.h
  ../src/oquam/SmoothPath.cpp
  ../include/oquam/ATDC.h
  ../src/oquam/ATDC.cpp
  ../include/oquam/Section.h
  ../src/oquam/Section.cpp
  ../include/oquam/print.h
  ../src/oquam/print.cpp
  ../src/oquam/is_valid.cpp
  ../include/oquam/is_valid.h
  )
add_executable(romi_rover_unit_tests_libromi_oquam_script ${SRCS_SCRIPT})
target_link_libraries(romi_rover_unit_tests_libromi_oquam_script rpp romi gtest gmock)

add_test(
    NAME romi_rover_unit_tests_libromi_oquam_script
    COMMAND romi_rover_unit_tests_libromi_oquam_script
)

##

set(SRCS_OQUAM
  src/tests_main.cpp
  src/tests_oquam.cpp
  mock/mock_cnccontroller.h
  ../include/oquam/CNCController.h
  ../include/oquam/Oquam.h
  ../src/oquam/Oquam.cpp
  ../src/oquam/plotter.cpp
  ../include/oquam/plotter.h
  ../include/oquam/print.h
  ../src/oquam/print.cpp
  ../src/oquam/is_valid.cpp
  ../include/oquam/is_valid.h
  ../include/oquam/Section.h
  ../src/oquam/Section.cpp
  ../include/oquam/ATDC.h
  ../src/oquam/ATDC.cpp
  ../include/oquam/SmoothPath.h
  ../src/oquam/SmoothPath.cpp
  ../src/oquam/Oquam.cpp
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
include_directories( "../src" )
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
            DEPENDENCIES romi_rover_unit_tests_libromi_v
                         romi_rover_unit_tests_libromi_cncrange
                         romi_rover_unit_tests_libromi_path
                         romi_rover_unit_tests_libromi_image
                         romi_rover_unit_tests_libromi_imageio
                         romi_rover_unit_tests_libromi_wheelodometry
                         romi_rover_unit_tests_libromi_navigationsettings
                         romi_rover_unit_tests_libromi_roveroptions
                         romi_rover_unit_tests_libromi_navigationadaptor
                         romi_rover_unit_tests_libromi_defaultnavigation
                         romi_rover_unit_tests_libromi_remotenavigation
                         romi_rover_unit_tests_libromi_crystaldisplay
                         romi_rover_unit_tests_libromi_joystick
                         romi_rover_unit_tests_libromi_joystickinputdevice
                         romi_rover_unit_tests_libromi_scriptlist
                         romi_rover_unit_tests_libromi_scriptmenu
                         romi_rover_unit_tests_libromi_roverscriptengine
                         romi_rover_unit_tests_libromi_statemachine
                         romi_rover_unit_tests_libromi_speedconverter
                         romi_rover_unit_tests_libromi_speedcontroller
                         romi_rover_unit_tests_libromi_scriptmenu
                         romi_rover_unit_tests_libromi_cncadaptor
                         romi_rover_unit_tests_libromi_steppersettings
                         romi_rover_unit_tests_libromi_oquam_section
                         romi_rover_unit_tests_libromi_oquam_script
                         romi_rover_unit_tests_libromi_oquam_print
                         romi_rover_unit_tests_libromi_oquam
                         romi_rover_unit_tests_libromi_brushmotordriver)
endif()

