
# Set Onyx Definitions
set(ONYX_PLATFORM_NAME ${CMAKE_SYSTEM_NAME})
message(STATUS "Building for ${ONYX_PLATFORM_NAME} version ${CMAKE_SYSTEM_VERSION}")


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Engine/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Testbed/")

# Only build Tests + Editor for Desktop. 
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests/")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Editor/")
endif()