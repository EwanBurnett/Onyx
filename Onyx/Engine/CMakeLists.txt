project("Onyx-Engine")

add_library(${PROJECT_NAME} STATIC
    "Onyx.h" 
    "Onyx.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})