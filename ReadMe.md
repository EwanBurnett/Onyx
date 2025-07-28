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

## Build Instructions
### Documentation 
- The Latest Version of Doxygen

 
### Windows
#### Visual Studio 2022 with CMake (Recommended)
Requirements:
- Visual Studio 2022 or higher. 
    - Other versions might work!
- CMake for Visual Studio Installed
- [Optional] The latest version of the Vulkan SDK 
    - If not installed, Onyx will default to rendering via DirectX 12.  

#### CMake (Standalone) 
- CMake v3.22 or higher
- [Optional] The latest version of the Vulkan SDK 
    - If not installed, Onyx will default to rendering via DirectX 12.  


### Linux
#### CMake (Standalone)
Requirements: 
- The latest version of the Vulkan SDK. 
- CMake v3.22 or higher


### Android
- Currently, only the Testbed project supports Android builds. 
    - This is planned to be resolved during a later version, adding support for custom projects.

#### Android Studio (Recommended)
Requirements: 
- The latest version of Android Studio. 
- The latest version of the Vulkan SDK. 

Steps: 
- Open `Onyx/Testbed/android/` in Android Studio 
- Execute a Gradle Sync.
- Build the project.

