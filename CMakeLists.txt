cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

cmake_policy(SET CMP0054 NEW)

# Project
project (FanSpeedController)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE   "Debug")

endif ()

set (CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}"  CACHE STRING "${CMAKE_BUILD_TYPE}" FORCE)

message (STATUS "Build Type - ${CMAKE_BUILD_TYPE}.")
message (STATUS "Host System - ${CMAKE_HOST_SYSTEM_NAME}.")
message (STATUS "Host Processor - ${CMAKE_HOST_SYSTEM_PROCESSOR}.")
message (STATUS "Target System - ${CMAKE_SYSTEM_NAME}.")
message (STATUS "Target Processor - ${CMAKE_SYSTEM_PROCESSOR}.")

# Output path.
set (OUTPUT_SUB_DIR "${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OUTPUT_SUB_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_SUB_DIR}")

# Firmware.
message (NOTICE     "Configuring firmware...")
file (MAKE_DIRECTORY        "${CMAKE_CURRENT_BINARY_DIR}/firmware")

execute_process (
    COMMAND             "cmake" "-DCMAKE_SYSTEM_NAME=Generic" 
                        "-DCMAKE_C_COMPILER=sdcc" 
                        "${CMAKE_CURRENT_SOURCE_DIR}/firmware"
                        "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" 
                        "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" 
                        "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    WORKING_DIRECTORY   "${CMAKE_CURRENT_BINARY_DIR}/firmware"
    RESULT_VARIABLE     result)

if (NOT result EQUAL 0)
    message (FATAL_ERROR    "Failed to configure firmware.")

endif ()

add_custom_target (firmware ALL
    COMMAND             "cmake" "--build" "."
    WORKING_DIRECTORY   "${CMAKE_CURRENT_BINARY_DIR}/firmware")
