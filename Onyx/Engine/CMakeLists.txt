# CMake Project File for the Engine Static Library. 
# ---------------------------------------------------
# Author: Ewan Burnett (EwanBurnettSK@Outlook.com)
# Created: 2025/07/26
# Last Modified: 2025/07/27

project("Onyx-Engine")

# Configure the Project
set(CMAKE_CXX_STANDARD 17)  # Target C++ 17

set(ONYX_ENGINE_LIBRARIES )

# Fetch Dependencies
include(FetchContent)

# Threads
find_package(Threads REQUIRED)
set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} Threads::Threads)


# Vulkan
find_package(Vulkan)
if(NOT Vulkan_FOUND)
    message(WARNING "Unable to find Vulkan SDK!")
    set(ONYX_ENABLE_VULKAN 0)
else()
    message(STATUS "Enabling Vulkan SDK.")
    set(ONYX_ENABLE_VULKAN 1)


    # Vulkan Memory Allocator
    FetchContent_Declare(
        vma
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
        GIT_TAG v3.2.0
    )
    # Disable VMA sample builds by default
    set(VMA_BUILD_SAMPLES OFF)

    FetchContent_MakeAvailable(vma)

    set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} Vulkan::Vulkan VulkanMemoryAllocator)
endif()

# DX12 (Shipped in Windows SDK)
if(WIN32)
    message(STATUS "Enabling DirectX 12 SDK.")
    set(ONYX_ENABLE_DX12 1)
    set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} d3d12.lib dxgi.lib dxguid.lib)
else()
    message(WARNING "Unable to find DirectX 12 SDK!")
    set(ONYX_ENABLE_DX12 0)
endif()

# Build GLFW for Desktop only!
if(WIN32 OR LINUX)
    # GLFW 
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        FIND_PACKAGE_ARGS NAMES glfw
    )
    FetchContent_MakeAvailable(glfw)

    set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} glfw)
endif()


# GLM 
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    FIND_PACKAGE_ARGS NAMES glm
)
 
# ASSIMP 
FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v6.0.2
)

# STB
FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb 
)

# SLang 
FetchContent_Declare(
    slang
    GIT_REPOSITORY https://github.com/shader-slang/slang
    GIT_TAG v2025.13.1 
)

# OpenAL Soft
FetchContent_Declare(
	openal_soft 
	GIT_REPOSITORY https://github.com/kcat/openal-soft
	GIT_TAG 1.24.3
)


# TODO: ImGui, ImGuizmo, ImPlot

FetchContent_MakeAvailable(glm assimp stb slang openal_soft)


# Configure the Versioning file. 
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/Onyx/Version.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/Onyx/Version.h")

# Build the Engine Static Library. 
add_library(${PROJECT_NAME} STATIC
    "include/Onyx/Onyx.h" 
    "src/Onyx.cpp"
    "include/Onyx/Version.h"
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${ONYX_ENGINE_LIBRARIES} 
    glm::glm 
    assimp::assimp 
    slang
    OpenAL::OpenAL
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/" 
    ${stb_INCLUDE_DIR}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC 
    ONYX_ENABLE_VULKAN=${ONYX_ENABLE_VULKAN} 
    ONYX_ENABLE_DX12=${ONYX_ENABLE_DX12}
)