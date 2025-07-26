# CMake Project File for the Unit Test application. 
# Runs Unit Tests via Google Test 
# ---------------------------------------------------
# Author: Ewan Burnett (EwanBurnettSK@Outlook.com)
# Created: 2025/07/26
# Last Modified: 2025/07/26

project("Tests")

include(FetchContent)
 
# Fetch Google Test
FetchContent_Declare(
    googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723 # release v1.17.0 
    FIND_PACKAGE_ARGS NAMES gtest 
)

# Windows only: Prevent gtest from overriding the project's compiler / linker settings.
if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} main.cpp)

# Ensure there is a build dependency on the main Engine library. 
add_dependencies(${PROJECT_NAME} Onyx-Engine)

# Link the main Engine to the testbed project. 
target_link_libraries(${PROJECT_NAME} Onyx-Engine GTest::gtest)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})