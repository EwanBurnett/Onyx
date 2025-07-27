# CMake Project File for the Engine Static Library. 
# ---------------------------------------------------
# Author: Ewan Burnett (EwanBurnettSK@Outlook.com)
# Created: 2025/07/26
# Last Modified: 2025/07/26

project("Onyx-Engine")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/Onyx/Version.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/Onyx/Version.h")

add_library(${PROJECT_NAME} STATIC
    "include/Onyx/Onyx.h" 
    "src/Onyx.cpp"
    "include/Onyx/Version.h"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
