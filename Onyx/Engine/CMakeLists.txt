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

# Desktop Only Libraries 
if(WIN32 OR LINUX)
    # GLFW 
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        FIND_PACKAGE_ARGS NAMES glfw
    )
     
    # Native File Dialog Extended
    FetchContent_Declare(
        nfd
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended
        GIT_TAG v1.2.1
    )

    FetchContent_MakeAvailable(glfw nfd)

    set(ONYX_ENGINE_LIBRARIES ${ONYX_ENGINE_LIBRARIES} glfw nfd)
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
    FIND_PACKAGE_ARGS NAMES assimp
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
    FIND_PACKAGE_ARGS NAMES slang
)

# OpenAL Soft
FetchContent_Declare(
	openal_soft 
	GIT_REPOSITORY https://github.com/kcat/openal-soft
	GIT_TAG 1.24.3
)

FetchContent_MakeAvailable(glm assimp stb slang openal_soft)

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
    target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR})

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
    ONYX_ENABLE_IMGUI=${ONYX_ENABLE_IMGUI}
)