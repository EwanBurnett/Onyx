# CMake Project File for the Engine Static Library. 
# ---------------------------------------------------
# Author: Ewan Burnett (EwanBurnettSK@Outlook.com)
# Created: 2025/07/26
# Last Modified: 2025/07/28

project("Onyx-Engine")

# Configure the Project
set(CMAKE_CXX_STANDARD 17)  # Target C++ 17

set(ONYX_ENGINE_LIBRARIES ) 

#TODO: Move to a CMake Option!
set(ONYX_ENABLE_IMGUI 1)


# Fetch Dependencies
include(FetchContent)

# Threads
find_package(Threads REQUIRED)
set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} Threads::Threads)
set(ONYX_ENGINE_SOURCES #define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
)
set(ONYX_ENGINE_INCLUDE_DIRS )

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

# Android 
if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    message(STATUS "Building for Android.")
    set(ONYX_PLATFORM_ANDROID 1)

    find_package(game-activity REQUIRED CONFIG)

    # Link Android Libraries 
    set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} game-activity::game-activity_static android log)

    # TODO: DO we need these???
    set(ONYX_ENGINE_SOURCES "${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c")
    set(ONYX_ENGINE_INCLUDE_DIRS ${ONYX_ENGINE_INCLUDE_DIRS} "${ANDROID_NDK}/sources/android/native_app_glue")
endif()

# Desktop Only Libraries 
if(WIN32 OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(ONYX_PLATFORM_DESKTOP 1)

    # GLFW 
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        # FIND_PACKAGE_ARGS NAMES glfw
    )
     
    # Native File Dialog Extended
    FetchContent_Declare(
        nfd
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended
        GIT_TAG v1.2.1
    )

    # SLang 
    FetchContent_Declare(
        slang
        GIT_REPOSITORY https://github.com/shader-slang/slang
        GIT_TAG v2025.13.1 
        # FIND_PACKAGE_ARGS NAMES slang
    )

    FetchContent_MakeAvailable(glfw nfd) #slang)

    set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} glfw nfd) #slang)
endif()


# GLM 
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    # FIND_PACKAGE_ARGS NAMES glm
)
 
# ASSIMP 
FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v6.0.2
    # FIND_PACKAGE_ARGS NAMES assimp
)

# STB
FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb 
)
set(ONYX_ENGINE_INCLUDE_DIRS ${ONYX_ENGINE_INCLUDE_DIRS} ${stb_INCLUDE_DIR})



# OpenAL Soft
FetchContent_Declare(
	openal_soft 
	GIT_REPOSITORY https://github.com/kcat/openal-soft
	GIT_TAG 1.24.3
)

FetchContent_MakeAvailable(glm assimp stb openal_soft)

# ImGui libraries
if(ONYX_ENABLE_IMGUI)
    message(STATUS "Building ImGui")
    # ImGui
    FetchContent_Declare(
        imgui   
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.7
    )

    # ImGuizmo 
    FetchContent_Declare(
        imguizmo 
        GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo
    )

    # ImPlot 
    FetchContent_Declare(
        implot 
        GIT_REPOSITORY https://github.com/epezent/implot
    )

    FetchContent_MakeAvailable(imgui imguizmo implot)
    
    set(IMGUI_PLATFORM_SOURCES )
    set(IMGUI_PLATFORM_LIBRARIES )

    if(ONYX_ENABLE_VULKAN)
        set(IMGUI_PLATFORM_SOURCES ${IMGUI_PLATFORM_SOURCES} "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp")
        set(IMGUI_PLATFORM_LIBRARIES ${IMGUI_PLATFORM_LIBRARIES} Vulkan::Vulkan)
    endif()

    if(ONYX_ENABLE_DX12)
        set(IMGUI_PLATFORM_SOURCES ${IMGUI_PLATFORM_SOURCES} "${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp")
        set(IMGUI_PLATFORM_LIBRARIES ${IMGUI_PLATFORM_LIBRARIES} d3d12.lib dxgi.lib dxguid.lib)
    endif()

    if(ONYX_PLATFORM_DESKTOP)
        set(IMGUI_PLATFORM_SOURCES ${IMGUI_PLATFORM_SOURCES} "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
        set(IMGUI_PLATFORM_LIBRARIES ${IMGUI_PLATFORM_LIBRARIES} glfw)
    elseif(ONYX_PLATFORM_ANDROID) 
        set(IMGUI_PLATFORM_SOURCES ${IMGUI_PLATFORM_SOURCES} "${imgui_SOURCE_DIR}/backends/imgui_impl_android.cpp" )

        set(IMGUI_PLATFORM_LIBRARIES ${IMGUI_PLATFORM_LIBRARIES} android log)

        set(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate"
        )

    endif()

    # Manually Build ImGui as a static library
    add_library(imgui_lib 
        "${imgui_SOURCE_DIR}/imgui.cpp" 
        "${imgui_SOURCE_DIR}/imgui_demo.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp" 
        "${imgui_SOURCE_DIR}/imgui_tables.cpp" 
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp" 
        ${IMGUI_PLATFORM_SOURCES}
    )

    target_link_libraries(imgui_lib PRIVATE ${IMGUI_PLATFORM_LIBRARIES})
    if(ONYX_PLATFORM_ANDROID)
        target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR} "${ANDROID_NDK}/sources/android/native_app_glue/")
    else()
        target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR})
    endif()

    # Manually build ImGuizmo too 
    add_library(imguizmo
        "${imguizmo_SOURCE_DIR}/ImGuizmo.cpp"
        "${imguizmo_SOURCE_DIR}/ImSequencer.cpp"
        "${imguizmo_SOURCE_DIR}/ImCurveEdit.cpp"
        "${imguizmo_SOURCE_DIR}/ImGradient.cpp"
        "${imguizmo_SOURCE_DIR}/GraphEditor.cpp"
    )
    target_link_libraries(imguizmo PUBLIC imgui_lib)
    target_include_directories(imguizmo PUBLIC ${imguizmo_SOURCE_DIR})

    # Manually build ImPlot as well 
    add_library(implot
        "${implot_SOURCE_DIR}/implot.cpp"
        "${implot_SOURCE_DIR}/implot_items.cpp"
        "${implot_SOURCE_DIR}/implot_demo.cpp"
    )
    target_link_libraries(implot PUBLIC imgui_lib)
    target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})

    set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} imgui_lib imguizmo implot)
endif()

# Version configuration 
set(ONYX_VERSION_MAJOR 0)
set(ONYX_VERSION_MINOR 0)
set(ONYX_VERSION_ISSUE 1)

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

# Configure the Versioning file. 
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/Onyx/Version.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/Onyx/Version.h")


# Build the Engine Static Library. 
add_library(${PROJECT_NAME} STATIC
    "include/Onyx/Onyx.h" 
    "src/Onyx.cpp"
    "include/Onyx/Version.h"
    "include/Onyx/Utility/Logger.h" 
    "src/Utility/Logger.cpp"
    "include/Onyx/Graphics/GPUDevice.h"
    "src/Graphics/GPUDevice.cpp"
    "include/Onyx/Graphics/Backends/GPUDevice_Vulkan.h"
    "src/Graphics/Backends/GPUDevice_Vulkan.cpp"   
 "include/Onyx/Graphics/Window.h" "src/Graphics/Window.cpp" "src/Graphics/Backends/VMAImplementation.cpp"
    "include/Onyx/Graphics/Buffer.h" "src/Graphics/Buffer.cpp"
 )

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${ONYX_ENGINE_LIBRARIES} 
    glm::glm 
    assimp::assimp 
    OpenAL::OpenAL
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/" 
    ${ONYX_ENGINE_INCLUDE_DIRS}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC 
    ONYX_ENABLE_VULKAN=${ONYX_ENABLE_VULKAN} 
    ONYX_ENABLE_DX12=${ONYX_ENABLE_DX12}
    ONYX_ENABLE_IMGUI=${ONYX_ENABLE_IMGUI}
)
