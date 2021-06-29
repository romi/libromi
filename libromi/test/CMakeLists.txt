
cmake_minimum_required(VERSION 3.10)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/mock
                     ${CMAKE_CURRENT_SOURCE_DIR}/support)

set(SRCS_V
        src/tests_main.cpp
        src/v_tests.cpp
  )

add_executable(libromi_unit_tests_v ${SRCS_V})
target_link_libraries(libromi_unit_tests_v romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_v
    COMMAND libromi_unit_tests_v
)

##

set(SRCS_CNC
        src/tests_main.cpp
        src/cncrange_tests.cpp
        src/cncadaptor_tests.cpp
  )
add_executable(libromi_unit_tests_cnc ${SRCS_CNC})
target_link_libraries(libromi_unit_tests_cnc romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_cnc
    COMMAND libromi_unit_tests_cnc
)

##

set(SRCS_CV
        src/tests_main.cpp
        src/cv_tests.cpp
        src/principal_component_analysis_tests.cpp
  )
add_executable(libromi_unit_tests_cv ${SRCS_CV})
target_link_libraries(libromi_unit_tests_cv romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_cv
    COMMAND libromi_unit_tests_cv
)

##

set(SRCS_IMAGER
        src/tests_main.cpp
        src/imager_tests.cpp
  )
add_executable(libromi_unit_tests_imager ${SRCS_IMAGER})
target_link_libraries(libromi_unit_tests_imager romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_imager
    COMMAND libromi_unit_tests_imager
)

##

set(SRCS_PATH
        src/tests_main.cpp
        src/path_tests.cpp
  )
add_executable(libromi_unit_tests_path ${SRCS_PATH})
target_link_libraries(libromi_unit_tests_path romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_path
    COMMAND libromi_unit_tests_path
)

##

set(SRCS_DATA_PROVIDER
        src/tests_main.cpp
        src/gps_location_provider_tests.cpp
        src/rover_identity_provider_tests.cpp
        src/session_tests.cpp
        src/metafolder_tests.cpp
        )

# The following tests exist to compare old code with new and are deprecated.
#        src/debug_data_dumper_tests.cpp

add_executable(libromi_unit_tests_data_provider ${SRCS_DATA_PROVIDER})
target_link_libraries(libromi_unit_tests_data_provider romi rpp rppmocks gtest gmock)

add_test(
        NAME libromi_unit_tests_data_provider
        COMMAND libromi_unit_tests_data_provider
)

##

set(SRCS_WHEELODOMETRY 
        src/tests_main.cpp
        src/wheelodometry_tests.cpp
  )

add_executable(libromi_unit_tests_wheelodometry ${SRCS_WHEELODOMETRY})
target_link_libraries(libromi_unit_tests_wheelodometry romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_wheelodometry
    COMMAND libromi_unit_tests_wheelodometry
)

##

set(SRCS_IMAGE_TESTS
        src/tests_main.cpp
        src/image_tests.cpp
        src/imageio_tests.cpp
        src/connected_components_tests.cpp
        support/test_utility.cpp
  )

add_executable(libromi_unit_tests_image ${SRCS_IMAGE_TESTS})
target_link_libraries(libromi_unit_tests_image romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_image
    COMMAND libromi_unit_tests_image
)

##

set(SRCS_ROVER
        src/tests_main.cpp
        src/roveroptions_tests.cpp
        src/roverscriptengine_tests.cpp
  )
add_executable(libromi_unit_tests_rover ${SRCS_ROVER})
target_link_libraries(libromi_unit_tests_rover romi rpp gtest gmock)

add_test(
    NAME libromi_unit_tests_rover
    COMMAND libromi_unit_tests_rover
)

##

set(SRCS_NAVIGATION
        src/tests_main.cpp
        src/locationtracker_tests.cpp
#        src/navigationsettings_tests.cpp
#        src/navigationadaptor_tests.cpp
#        src/remotenavigation_tests.cpp
#        src/navigation_tests.cpp
        )

add_executable(libromi_unit_tests_navigation ${SRCS_NAVIGATION})
target_link_libraries(libromi_unit_tests_navigation romi romi rpp rcompp-lite gtest gmock)

add_test(
    NAME libromi_unit_tests_navigation
    COMMAND libromi_unit_tests_navigation
)

##

set(SRCS_CRYSTALDISPLAY 
        src/tests_main.cpp
        src/crystaldisplay_tests.cpp
  )

add_executable(libromi_unit_tests_crystaldisplay ${SRCS_CRYSTALDISPLAY})
target_link_libraries(libromi_unit_tests_crystaldisplay rcompp-lite romi gtest gmock)

add_test(
    NAME libromi_unit_tests_crystaldisplay
    COMMAND libromi_unit_tests_crystaldisplay
)

##

set(SRCS_JOYSTICK 
        src/tests_main.cpp
        src/joystick_tests.cpp
        src/joystickinputdevice_tests.cpp
  )

add_executable(libromi_unit_tests_joystick ${SRCS_JOYSTICK})
target_link_libraries(libromi_unit_tests_joystick rpp rppmocks romi gtest gmock)

add_test(
    NAME libromi_unit_tests_joystick
    COMMAND libromi_unit_tests_joystick
)

##

set(SRCS_SCRIPTLIST 
        src/tests_main.cpp
        src/scriptlist_tests.cpp
  )

add_executable(libromi_unit_tests_scriptlist ${SRCS_SCRIPTLIST})
target_link_libraries(libromi_unit_tests_scriptlist rcompp-lite romi gtest gmock)

add_test(
    NAME libromi_unit_tests_scriptlist
    COMMAND libromi_unit_tests_scriptlist
)

##

set(SRCS_STATEMACHINE 
        src/tests_main.cpp
        src/statemachine_tests.cpp
  )

add_executable(libromi_unit_tests_statemachine ${SRCS_STATEMACHINE})
target_link_libraries(libromi_unit_tests_statemachine rcompp-lite romi gtest gmock)

add_test(
    NAME libromi_unit_tests_statemachine
    COMMAND libromi_unit_tests_statemachine
)

##

set(SRCS_SPEEDTESTS
  src/tests_main.cpp
        src/speedconverter_tests.cpp
        src/speedcontroller_tests.cpp
  )

add_executable(libromi_unit_tests_speed ${SRCS_SPEEDTESTS})
target_link_libraries(libromi_unit_tests_speed rpp romi gtest gmock)

add_test(
    NAME libromi_unit_tests_speed
    COMMAND libromi_unit_tests_speed
)

##

set(SRCS_SCRIPTMENU 
        src/tests_main.cpp
        src/scriptmenu_tests.cpp
  )

add_executable(libromi_unit_tests_scriptmenu ${SRCS_SCRIPTMENU})
target_link_libraries(libromi_unit_tests_scriptmenu rcompp-lite romi gtest gmock)

add_test(
    NAME libromi_unit_tests_scriptmenu
    COMMAND libromi_unit_tests_scriptmenu
)

##

set(SRCS_STEPPERSETTINGS
        src/tests_main.cpp
        src/steppersettings_tests.cpp
  )
add_executable(libromi_unit_tests_steppersettings ${SRCS_STEPPERSETTINGS})
target_link_libraries(libromi_unit_tests_steppersettings rpp romi gtest gmock)

add_test(
    NAME libromi_unit_tests_steppersettings
    COMMAND libromi_unit_tests_steppersettings
)

##

set(SRCS_OQUAM
        src/tests_main.cpp
        src/oquam_section_tests.cpp
        src/oquam_print_tests.cpp
        src/oquam_script_tests.cpp
        src/oquam_tests.cpp
  )
add_executable(libromi_unit_tests_oquam ${SRCS_OQUAM})
target_link_libraries(libromi_unit_tests_oquam rpp romi gtest gmock)

add_test(
    NAME libromi_unit_tests_oquam
    COMMAND libromi_unit_tests_oquam
)

##
  
set(SRCS_BRUSHMOTORDRIVER 
        src/tests_main.cpp
        src/brushmotordriver_tests.cpp
  )

add_executable(libromi_unit_tests_brushmotordriver ${SRCS_BRUSHMOTORDRIVER})
target_link_libraries(libromi_unit_tests_brushmotordriver romi romiserial rpp rcompp-lite gtest gmock)

add_test(
    NAME libromi_unit_tests_brushmotordriver
    COMMAND libromi_unit_tests_brushmotordriver
)

##
  
set(SRCS_RCOMMESSAGEHANDLER 
        src/tests_main.cpp
        src/rcommessagehandler_tests.cpp
  )

add_executable(libromi_unit_tests_rcommessagehandler ${SRCS_RCOMMESSAGEHANDLER})
target_link_libraries(libromi_unit_tests_rcommessagehandler romi romiserial rpp rcompp-lite rcom-mocks gtest gmock)

add_test(
    NAME libromi_unit_tests_rcommessagehandler
    COMMAND libromi_unit_tests_rcommessagehandler
)

##
  
set(SRCS_NAVIGATIONCONTROL 
        src/tests_main.cpp
        src/l1_navigationcontroller_tests.cpp
        src/pythontrackfollower_tests.cpp
)

add_executable(libromi_unit_tests_navigation_control ${SRCS_NAVIGATIONCONTROL})
target_link_libraries(libromi_unit_tests_navigation_control romi romiserial rpp rcompp-lite rcom-mocks gtest gmock)

add_test(
    NAME libromi_unit_tests_navigation_control
    COMMAND libromi_unit_tests_navigation_control
)

##

Message (${CMAKE_BINARY_DIR})

if(BUILD_COVERAGE)
    SETUP_TARGET_FOR_COVERAGE_LCOV(
            NAME libromi_unit_tests_coverage
            EXECUTABLE ctest -V ${n_cores} -R "libromi"
            EXCLUDE '*wrapper*' "src/Linux.cpp" "include/*" '${CMAKE_BINARY_DIR}/libr/*'
            DEPENDENCIES    libromi_unit_tests_v
                            libromi_unit_tests_cnc
                            libromi_unit_tests_cv
                            libromi_unit_tests_imager
                            libromi_unit_tests_path
                            libromi_unit_tests_navigation
                            libromi_unit_tests_wheelodometry
                            libromi_unit_tests_image
                            libromi_unit_tests_rover
                            libromi_unit_tests_navigation
                            libromi_unit_tests_crystaldisplay
                            libromi_unit_tests_joystick
                            libromi_unit_tests_scriptlist
                            libromi_unit_tests_statemachine
                            libromi_unit_tests_speed
                            libromi_unit_tests_scriptmenu
                            libromi_unit_tests_steppersettings
                            libromi_unit_tests_oquam
                            libromi_unit_tests_brushmotordriver
                            libromi_unit_tests_rcommessagehandler
                            libromi_unit_tests_navigation_control
    )
endif()

SET(TEST_DATA_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_data)

add_custom_command(TARGET libromi_unit_tests_image POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_DATA_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy test_data/*.* ${TEST_DATA_DIR}
        WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
        libromi_unit_tests_image
        COMMENT
        "Making directory ${TEST_DATA_DIR} and copying test files."
        )

