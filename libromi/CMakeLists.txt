cmake_minimum_required(VERSION 3.10)
project(libromi)

set(romi_VERSION_MAJOR 0)
set(romi_VERSION_MINOR 1)

MESSAGE("libromi CXX Flags = ${CMAKE_CXX_FLAGS}")
MESSAGE("libromi C Flags = ${CMAKE_C_FLAGS}")

############################################################

if((CMAKE_SYSTEM_PROCESSOR MATCHES ".*arm") OR (CMAKE_SYSTEM_PROCESSOR MATCHES ".*aarch64"))
    set(PICAMERA_SOURCES
      include/picamera/CameraComponent.h
      include/picamera/Component.h
      include/picamera/Connection.h
      include/picamera/EncoderComponent.h
      include/picamera/MMAL.h
      include/picamera/PiCamera.h
      include/picamera/PiCameraSettings.h
      include/picamera/Semaphore.h
      src/picamera/CameraComponent.cpp
      src/picamera/Component.cpp
      src/picamera/Connection.cpp
      src/picamera/EncoderComponent.cpp
      src/picamera/MMAL.cpp
      src/picamera/PiCamera.cpp
      src/picamera/PiCameraSettings.cpp
      src/picamera/Semaphore.cpp)
else()
   set(PICAMERA_SOURCES)
endif()
        
set(SOURCES

        include/IFileCabinet.h
        include/IFolder.h
        include/v3.h

        include/api/ActivityHelper.h
        include/api/EventTimer.h
        include/api/IEventTimer.h
        include/api/CNCRange.h
        include/api/IActivity.h
        include/api/ICamera.h
        include/api/ICNC.h
        include/api/IDisplay.h
        include/api/IEventSource.h
        include/api/IGimbal.h
        include/api/IInputDevice.h
        include/api/IMotorDriver.h
        include/api/INavigation.h
        include/api/INotifications.h
        include/api/IPowerDevice.h
        include/api/IWeeder.h
        include/api/Joystick.h
        include/api/Path.h
        include/api/StateMachine.h

        include/camera/FileCamera.h
        include/camera/USBCamera.h

        include/configuration/ConfigurationProvider.h
        include/configuration/IOptions.h
        include/configuration/GetOpt.h

        include/cv/ImageCropper.h
        include/cv/IImageCropper.h
        include/cv/Image.h
        include/cv/ImageIO.h

        include/fake/FakeCNC.h
        include/fake/FakeDisplay.h
        include/fake/FakeInputDevice.h
        include/fake/FakeMotorDriver.h
        include/fake/FakeNavigation.h
        include/fake/FakeNotifications.h
        include/fake/FakeScriptEngine.h
        include/fake/FakeWeeder.h

        include/hal/BldcGimbal.h
        include/hal/BrushMotorDriver.h

        include/notifications/FluidSoundNotifications.h

        include/oquam/ATDC.h
        include/oquam/ICNCController.h
        include/oquam/FakeCNCController.h
        include/oquam/is_valid.h
        include/oquam/Oquam.h
        include/oquam/OquamSettings.h
        include/oquam/plotter.h
        include/oquam/print.h
        include/oquam/SmoothPath.h
        include/oquam/Section.h
        include/oquam/StepperController.h
        include/oquam/StepperSettings.h

        include/rover/EventsAndStates.h
        include/rover/Rover.h
        include/rover/RoverInterface.h
        include/rover/RoverNotifications.h
        include/rover/RoverOptions.h
        include/rover/RoverScriptEngine.h
        include/rover/RoverStateMachine.h
        include/rover/Navigation.h
        include/rover/SpeedController.h
        include/rover/NavigationSettings.h
        include/rover/ISpeedController.h
        include/rover/SpeedConverter.h
        include/rover/WheelOdometry.h

        include/rpc/CameraAdaptor.h
        include/rpc/CNCAdaptor.h
        include/rpc/GimbalAdaptor.h
        include/rpc/IRPCClient.h
        include/rpc/IRPCHandler.h
        include/rpc/IRPCServer.h
        include/rpc/MethodsActivity.h
        include/rpc/MethodsCamera.h
        include/rpc/MethodsCNC.h
        include/rpc/MethodsGimbal.h
        include/rpc/MethodsNavigation.h
        include/rpc/MethodsRover.h
        include/rpc/MethodsWeeder.h
        include/rpc/NavigationAdaptor.h
        include/rpc/RcomClient.h
        include/rpc/RcomMessageHandler.h
        include/rpc/RcomServer.h
        include/rpc/RemoteCamera.h
        include/rpc/RemoteCNC.h
        include/rpc/RemoteGimbal.h
        include/rpc/RemoteNavigation.h
        include/rpc/RemoteStub.h
        include/rpc/RemoteWeeder.h
        include/rpc/WeederAdaptor.h

        include/ui/CrystalDisplay.h
        include/ui/JoystickInputDevice.h
        include/ui/JoystickEventMapper.h
        include/ui/LinuxJoystick.h
        include/ui/Menu.h
        include/ui/IScriptEngine.h
        include/ui/ScriptList.h
        include/ui/ScriptMenu.h
        include/ui/UIEventMapper.h

        src/api/CNCRange.cpp
        src/api/Path.cpp
        src/api/ActivityHelper.cpp
        src/api/EventTimer.cpp

        src/hal/BldcGimbal.cpp
        src/hal/BrushMotorDriver.cpp

        src/configuration/ConfigurationProvider.cpp
        src/configuration/GetOpt.cpp

        src/cv/Image.cpp
        src/cv/ImageIO.cpp
        src/cv/ImageCropper.cpp

        src/camera/FileCamera.cpp
        src/camera/USBCamera.cpp
        src/camera/camera_v4l.h
        src/camera/camera_v4l.c

        src/fake/FakeScriptEngine.cpp

        src/notifications/FluidSoundNotifications.cpp

        src/oquam/ATDC.cpp
        src/oquam/is_valid.cpp
        src/oquam/Oquam.cpp
        src/oquam/OquamSettings.cpp
        src/oquam/plotter.cpp
        src/oquam/print.cpp
        src/oquam/SmoothPath.cpp
        src/oquam/Section.cpp
        src/oquam/StepperSettings.cpp
        src/oquam/StepperController.cpp

        src/rover/EventsAndStates.cpp
        src/rover/Rover.cpp
        src/rover/RoverInterface.cpp
        src/rover/RoverOptions.cpp
        src/rover/RoverScriptEngine.cpp
        src/rover/RoverStateMachine.cpp
        src/rover/Navigation.cpp
        src/rover/SpeedController.cpp
        src/rover/SpeedConverter.cpp
        src/rover/WheelOdometry.cpp

        src/rpc/CameraAdaptor.cpp
        src/rpc/CNCAdaptor.cpp
        src/rpc/GimbalAdaptor.cpp
        src/rpc/NavigationAdaptor.cpp
        src/rpc/RcomClient.cpp
        src/rpc/RcomMessageHandler.cpp
        src/rpc/RcomServer.cpp
        src/rpc/RemoteCamera.cpp
        src/rpc/RemoteCNC.cpp
        src/rpc/RemoteGimbal.cpp
        src/rpc/RemoteNavigation.cpp
        src/rpc/RemoteStub.cpp
        src/rpc/RemoteWeeder.cpp
        src/rpc/WeederAdaptor.cpp

        src/ui/CrystalDisplay.cpp
        src/ui/JoystickInputDevice.cpp
        src/ui/LinuxJoystick.cpp
        src/ui/UIEventMapper.cpp
        src/ui/ScriptList.cpp
        src/ui/ScriptMenu.cpp

        src/v3.cpp
        )

set(DATA_PROVIDER_SOURCES
        include/data_provider/JsonFieldNames.h
        include/data_provider/IGPS.h
        include/data_provider/ILocationProvider.h
        include/data_provider/GpsLocationProvider.h
        src/data_provider/GpsLocationProvider.cpp
        include/data_provider/IIdentityProvider.h
        include/data_provider/RoverIdentityProvider.h
        src/data_provider/RoverIdentityProvider.cpp
        include/data_provider/ISoftwareVersion.h
        include/data_provider/SoftwareVersion.h
        src/data_provider/SoftwareVersion.cpp
        include/data_provider/RomiDeviceData.h
        src/data_provider/RomiDeviceData.cpp
        src/session/Session.cpp
        include/session/Session.h
        include/session/MetaFolder.h
        include/session/IMetaFolder.h
        src/session/MetaFolder.cpp
        src/data_provider/Gps.cpp
        include/data_provider/Gps.h
        include/debug_tools/debug_data_dumper.h
        src/debug_tools/debug_data_dumper.cpp)


set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/weeder/som/SOM.cpp PROPERTIES COMPILE_FLAGS -O3)
#set_source_files_properties(${NO_CONV_SOURCES} PROPERTIES COMPILE_FLAGS -Wno-conversion)
#set_source_files_properties(${NO_CONV_SOURCES} PROPERTIES COMPILE_FLAGS -Wno-sign-conversion)
#set_source_files_properties(${NO_CONV_SOURCES} PROPERTIES COMPILE_FLAGS -Wno-float-conversion)

add_library(romi SHARED ${SOURCES} ${DATA_PROVIDER_SOURCES} ${PICAMERA_SOURCES})

target_include_directories(romi
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        ${ORTOOLS_INCLUDE_DIR}
        ${PICAMERA_INCLUDE_DIR}
        )

target_link_libraries(romi
        romiserial
        r
        rpp
        rcompp-lite
        fluidsynth
        jpeg
        png
        ${ORTOOLS_SHARED_LIB}
        ${PICAMERA_SHARED_LIB})

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

add_custom_command(TARGET romi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${ORTOOLS_SHARED_LIB_DIR}/*.so ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        DEPENDS romi
        COMMENT "Copying binary ortools libs to ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} for distribution."
        )
