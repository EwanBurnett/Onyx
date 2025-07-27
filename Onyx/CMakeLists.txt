
# Set Onyx Definitions
set(ONYX_PLATFORM_NAME ${CMAKE_SYSTEM_NAME})
message(STATUS "Building for ${ONYX_PLATFORM_NAME} version ${CMAKE_SYSTEM_VERSION}")

# TODO: Version configuration 
set(ONYX_VERSION_MAJOR 0)
set(ONYX_VERSION_MINOR 0)
set(ONYX_VERSION_ISSUE 0)

set(ONYX_GIT_BRANCH "")
set(ONYX_GIT_COMMIT_HASH "")

# TODO: Move to a .cmake script! 
find_package(Git)
if(Git_FOUND)
    # Retrieve the Branch name from Git. 
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE ONYX_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Git Branch ${ONYX_GIT_BRANCH}")

    # Retrieve the latest Commit hash from Git. 
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --format=%h 
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE ONYX_GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Git Commit Hash ${ONYX_GIT_COMMIT_HASH}")

else()
    message(WARNING "Git Executable not found!")
endif()


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Engine/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Testbed/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests/")