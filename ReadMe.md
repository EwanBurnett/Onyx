# Onyx 

## Overview
A 3D Cross-Platform Game Engine, Written in C++ 17 for Windows, Linux and Android. 

- Platform Support
    - Windows 10+ 
    - Linux (Tested on Ubuntu 22.04) 
    - Android API 30 (Android 11.0+) 

### File Structure
```
.
├── Docs/               // Documentation Files
│   └── Doxyfile
├── Onyx/               // Onyx Core
│   ├── Editor/         // Onyx Editor
│   │   └── ...
│   ├── Engine/         // Onyx Engine
│   │   └── ...
│   ├── Testbed/        // Testbed Application
│   │   ├── android/    // Android build files
│   │   └── ...
│   ├── Tests/          // Unit Tests
│   │   └── ...
│   └── Tools/          // Standalone Tools
│       └── ... 
├── Resources/          // Common Resources
│   └── ...
└── ...
```

### SDKs
| Name | Version | 
| - | - | 
| [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) | 10.0.26100+ |
| [Vulkan SDK](https://vulkan.lunarg.com) | 1.2+ |
| [Android SDK + NDK](https://developer.android.com/studio) | |


### External Libraries
| Name | Description | Version | License |
| - | - | - | - |
| [Google Test](https://github.com/google/googletest.git) | Unit Testing Framework | v1.17.0 | [BSD-3-Clause License](https://github.com/google/googletest/blob/main/LICENSE) |
| [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) | Vulkan Memory Allocation backend | v3.2.0 | [MiT License](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/blob/master/LICENSE.txt) |
| [GLFW](https://github.com/glfw/glfw.git) | Vulkan Memory Allocation backend | 3.4 | [Zlib License](https://github.com/glfw/glfw/blob/master/LICENSE.md) |
| [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended) | Platform Independent File Dialog | v1.2.1 | [Zlib License](https://github.com/btzy/nativefiledialog-extended/blob/master/LICENSE) |
| [Slang](https://github.com/shader-slang/slang) | Modern Vendor-agnostic Shader Language | v2025.13.1 | [Apache-2.0 WITH LLVM-exception](https://github.com/shader-slang/slang/blob/master/LICENSE) |
| [glm](https://github.com/g-truc/glm.git) | Linear Algebra Library | 1.0.1 | [MiT License](https://github.com/g-truc/glm?tab=License-1-ov-file#readme) |
| [assimp](https://github.com/assimp/assimp.git) | Asset Importer | v6.0.2 | [Proprietary License](https://github.com/assimp/assimp/blob/master/LICENSE) |
| [stb](https://github.com/nothings/stb ) | Image Utilities | v6.0.2 | [MiT License](https://github.com/nothings/stb/blob/master/LICENSE) |
| [OpenAL soft](https://github.com/kcat/openal-soft) | Cross-Platform Audio Library | 1.24.3 | [Proprietary License](https://github.com/kcat/openal-soft/blob/master/LICENSE-pffft) |
| [ImGui](https://github.com/ocornut/imgui.git) | Immediate Mode Graphial User Interface | v1.91.7 | [MiT License](https://github.com/ocornut/imgui/blob/master/LICENSE.txt) |
| [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) | Gizmos for ImGui | HEAD | [MiT License](https://github.com/CedricGuillemet/ImGuizmo/blob/master/LICENSE) |
| [ImPlot](https://github.com/epezent/implot) | Plotting Library for ImGui | HEAD | [MiT License](https://github.com/epezent/implot/blob/master/LICENSE) |

## Build Instructions

### Cloning
Clone the Repository from Git. 
```git
git clone https://EwanBurnett/Onyx.git 
cd Onyx/
```

### Documentation 
- The Latest Version of [Doxygen](https://www.doxygen.nl/index.html)

 
### Windows
#### Visual Studio 2022 with CMake (Recommended)
Requirements:
- Visual Studio 2022 or higher. 
    - Other versions might work!
- CMake for Visual Studio Installed
- [Optional] The latest version of the Vulkan SDK 
    - If not installed, Onyx will default to rendering via DirectX 12.  

Steps: 
1. Open the Repository Folder in Visual Studio 2022. 
2. Select a Build Configuration, and build as usual. 

#### CMake (Standalone) 
- CMake v3.22 or higher
- [Optional] The latest version of the Vulkan SDK 
    - If not installed, Onyx will default to rendering via DirectX 12.

Steps: 
1. Create a `Build` directory
```cmd
mkdir build && cd build
```
2. Run CMake from the `Build` directory. 
```cmd
cmake .. 
```
3. Build the project from the generated files.

### Linux
#### CMake (Standalone)
Requirements: 
- The latest version of the Vulkan SDK. 
- CMake v3.22 or higher


### Android
- Currently, only the Testbed project supports Android builds. 
    - This is planned to be resolved during a later version, adding support for custom projects.
- Note: To use Vulkan Validation layers in Android, follow [this](https://developer.android.com/ndk/guides/graphics/validation-layer) guide.

#### Android Studio (Recommended)
Requirements: 
- The latest version of Android Studio. 
- The latest version of the Vulkan SDK. 

Steps: 
- Open `Onyx/Testbed/android/` in Android Studio 
- Execute a Gradle Sync.
- Build the project.

