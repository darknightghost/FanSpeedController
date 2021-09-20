cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)

cmake_policy(SET CMP0054 NEW)

# sdcc
find_program (SDCC_EXECUTABLE
    sdcc)

if (SDCC_EXECUTABLE)
    message (STATUS "SDCC found - ${SDCC_EXECUTABLE}")

else ()
    message (FATAL_ERROR    "Missing SDCC")

endif ()

set ("CMAKE_SYSTEM_NAME"    "Generic")
set ("CMAKE_C_COMPILER"     "${SDCC_EXECUTABLE}")

# Project
project (FanSpeedControllerFirmware  C)

set (DATA_SIZE              128)
set (IDATA_SIZE             128)
set (XDATA_SIZE             1024)

add_compile_options (
    "--std-sdcc11" 
    "--use-non-free" 
    "--fsigned-char" 
    "-mmcs51" 
    "--iram-size" "${IDATA_SIZE}"
    "--xram-size" "${XDATA_SIZE}"
    "-DBUILD_FIRMWARE"
    )

include_directories (
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )

file (GLOB_RECURSE      SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c"
    )

add_executable ("${PROJECT_NAME}"
    "${SRC}")

add_link_options (
    "-mmcs51" 
    "--iram-size" "${IDATA_SIZE}"
    "--xram-size" "${XDATA_SIZE}"
    )

add_custom_command (
    DEPENDS     "${PROJECT_NAME}"
    OUTPUT      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex"
    COMMAND     "bash" "-c" "packihx \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.ihx\" >\"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex\""
    )

add_custom_target (
    "${PROJECT_NAME}.hex"   ALL
    DEPENDS     "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex"
    )
