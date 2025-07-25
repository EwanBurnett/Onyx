# CMake Project File for the Testbed application. 
# This application is used to test Engine features. 
# ---------------------------------------------------
# Author: Ewan Burnett (EwanBurnettSK@Outlook.com)
# Created: 2025/07/26
# Last Modified: 2025/07/26

project("Testbed")

add_executable(${PROJECT_NAME} main.cpp)

# Ensure there is a build dependency on the main Engine library. 
add_dependencies(${PROJECT_NAME} Onyx-Engine)

# Link the main Engine to the testbed project. 
target_link_libraries(${PROJECT_NAME} Onyx-Engine)