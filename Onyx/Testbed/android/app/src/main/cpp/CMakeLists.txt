# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("testbed-android")

set(CMAKE_DEBUG_POSTFIX "")
add_subdirectory("../../../../../../Engine/" "build/Engine/")  # Build the main engine
add_subdirectory("../../../../../../Testbed/" "build/Testbed/")  # Build the application
