project(binortools)
cmake_minimum_required(VERSION 3.10)

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set( ORTOOLS_LIB_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_SYSTEM_PROCESSOR})
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND (CMAKE_SYSTEM_PROCESSOR MATCHES ".*arm") )
    set( ORTOOLS_LIB_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/arm32)
#elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND (CMAKE_SYSTEM_PROCESSOR MATCHES ".*aarch64") )
#    set( ORTOOLS_LIB_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/arm64)
else()
    MESSAGE("Unsupported pre-built ortools")
endif()

find_path(ORTOOLS_INCLUDE_DIR
        NAMES ortools_export.h
        PATHS ${ORTOOLS_LIB_PATH_ROOT}/include
        )

find_library(ORTOOLS_SHARED_LIB
        NAMES ortools
        PATHS ${ORTOOLS_LIB_PATH_ROOT}/lib)

add_library(ortools SHARED IMPORTED GLOBAL)   
set_property(TARGET ortools 
                PROPERTY
                IMPORTED_LOCATION ${ORTOOLS_SHARED_LIB})    

set_property(TARGET ortools 
                PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES "${ORTOOLS_INCLUDE_DIR}"
         )