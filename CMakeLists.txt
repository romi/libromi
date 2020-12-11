cmake_minimum_required(VERSION 2.6)
project(libromi)

set(romi_VERSION_MAJOR 0)
set(romi_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default option is off.
# We override this on the server for a test buid, so tests should be run with this on before checking in.
option(ADDRESS_SANITISER_BUILD "Enable build settings to allow address sanitiser" ON) # default to off

if(ADDRESS_SANITISER_BUILD)

    message(WARNING "Enabling GCC address sanitiser will disable all Valgrind tasks due to compatibility errors")
    set( PROJECT_SANITISE_FLAGS
            "-fsanitize=address,undefined,shift,integer-divide-by-zero,unreachable,vla-bound,null,return,signed-integer-overflow,bounds,alignment,object-size,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,vptr")
else()
    MESSAGE("libr sanitize flags off")
    set( PROJECT_SANITISE_FLAGS "")
endif()

# For now everything is debug. Set Compile options locally to maintain independent library builds.
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic  -Werror ${PROJECT_SANITISE_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic  -Werror ${PROJECT_SANITISE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-O0")

#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} ${PROJECT_SANITISE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PROJECT_SANITISE_FLAGS}")

############################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

############################################################

set(CMAKE_MODULES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)
set(CMAKE_THIRDPARTY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# Options.
#Build tests by default. We want run tests whether we are developing or building a release. I mean... why wouldn't you? :)
option(BUILD_TESTS "Build all tests." ON)
option(BUILD_COVERAGE "Build coverage." ON)

if(BUILD_TESTS)

    include(CTest)
    enable_testing()

    if (NOT DEFINED googletest_SOURCE_DIR)
        if (CMAKE_VERSION VERSION_LESS 3.2)
            set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
        else()
            set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
        endif()

        include(${CMAKE_MODULES_DIRECTORY}/DownloadProject.cmake)
        download_project(   PROJ                googletest
                GIT_REPOSITORY      https://github.com/google/googletest.git
                GIT_TAG             master
                PREFIX              ${CMAKE_THIRDPARTY_DIRECTORY}/googletest
                ${UPDATE_DISCONNECTED_IF_AVAILABLE}
                )

        # Prevent GoogleTest from overriding our compiler/linker options
        # when building with Visual Studio
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

endif() # BUILD_TESTS

# Can't build for coverage without the test.
if (BUILD_COVERAGE AND BUILD_TESTS)
    if (NOT DEFINED lcov_SOURCE_DIR)
        include(${CMAKE_MODULES_DIRECTORY}/CodeCoverage.cmake)

        # lcov doesn't build with CMake so we download, set lcov as an external target, then call make.
        # we then manually add the bin path so that we can call it later.
        download_project(   PROJ              lcov
                URL               https://github.com/linux-test-project/lcov/releases/download/v1.15/lcov-1.15.tar.gz
                PREFIX            ${CMAKE_THIRDPARTY_DIRECTORY}/lcov
                TIMEOUT           180
                ${UPDATE_DISCONNECTED_IF_AVAILABLE}
                )

        set(LCOV_PATH "${lcov_SOURCE_DIR}/bin/lcov")
        set(GENHTML_PATH "${lcov_SOURCE_DIR}/bin/genhtml")
    endif()

    set( COVERAGE_EXCLUDES
            "/usr/*"
            "${CMAKE_SOURCE_DIR}/thirdparty/*"
            "*test/*")
    append_coverage_compiler_flags()

endif()

######################################

if (NOT DEFINED libr_SOURCE_DIR)
    set(LIBR_DIR "${CMAKE_SOURCE_DIR}/libr")
    find_path(LIBR_DIRECTORY
            NAMES "CMakeLists.txt"
            PATHS ${LIBR_DIR})
#    Save the current Build test settings so we can restore them after including libr. We don't want to build libr tests.
    SET(SAVED_TEST_OPTION BUILD_TESTS)
    SET(BUILD_TESTS OFF)

    if (NOT LIBR_DIRECTORY)
        MESSAGE("LIBROMI - ${LIBR_DIR} does not exist. Downloading dependency.")
        MESSAGE("LIBROMI - WARNING")
        MESSAGE("LIBROMI - The dependency is cloned to support rcom development only.")
        MESSAGE("LIBROMI - To work on both rcom and libr please see: https://github.com/romi/romi-rover-build-and-test")

#        Use this to get current branch from master project. Could use to pull projects instead of submodules.
#        git rev-parse --abbrev-ref HEAD

        download_project(
                PROJ              libr
                GIT_REPOSITORY    https://github.com/romi/libr.git
                GIT_TAG           ci_dev
                TIMEOUT           180
                SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/libr
                BINARY_DIR        ${CMAKE_CURRENT_BINARY_DIR}/libr
                ${UPDATE_DISCONNECTED_IF_AVAILABLE})
        add_subdirectory(${LIBR_DIR})
    else()
        MESSAGE("RCOM - ${LIBR_DIRECTORY} exists. Not downloading dependency.")
        add_subdirectory(libr)
    endif()
    # Restore build tests option.
    SET(BUILD_TESTS OFF SAVED_TEST_OPTION)
endif()

###########################################################3

set(SOURCES
  include/romi/CameraFactory.h
  include/romi/CameraFile.h
  include/romi/CameraProxy.h
  include/romi/CameraServer.h
  include/romi/CameraUSB.h
  include/romi/camera_v4l.h
  include/romi/CNCProxy.h
  include/romi/cnc_range.h
  include/romi/CNCRange.h
  include/romi/ConfigurationFile.h
  include/romi/FakeCNC.h
  include/romi/fsdb.h
  include/romi/ICamera.h
  include/romi/ICNC.h
  include/romi/IConfiguration.h
  include/romi/IFileCabinet.h
  include/romi/IFolder.h
  include/romi/image.h
  include/romi/Image.h
  include/romi/IRPCHandler.h
  include/romi/JSON.h
  include/romi/Path.h
  include/romi/quaternion.h
  include/romi/rover.h
  include/romi/RPCClient.h
  include/romi/RPCServer.h
  include/romi/vector.h
  src/vector.c 
  src/quaternion.c 
  src/image.c 
  src/fsdb.c 
  src/rover.c
  src/cnc_range.c
  src/CNCProxy.cpp
  src/RPCClient.cpp
  src/RPCServer.cpp
  src/CameraFactory.cpp
  src/CameraUSB.cpp
  src/camera_v4l.c
  )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

add_library(romi SHARED ${SOURCES})

target_include_directories(romi
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/romi"
        )

target_link_libraries(romi
                        r
                        librcom
                        jpeg
                        png
                        m)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

INSTALL(TARGETS romi DESTINATION "lib")
INSTALL (
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h")
