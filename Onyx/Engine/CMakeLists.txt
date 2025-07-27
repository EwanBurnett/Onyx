# CMake Project File for the Engine Static Library. 
# ---------------------------------------------------
# Author: Ewan Burnett (EwanBurnettSK@Outlook.com)
# Created: 2025/07/26
# Last Modified: 2025/07/26

project("Onyx-Engine")

add_library(${PROJECT_NAME} STATIC
    "Onyx.h" 
    "Onyx.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/Version.h")