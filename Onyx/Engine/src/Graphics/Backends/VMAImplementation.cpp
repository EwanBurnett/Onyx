/*
* @file VMAImplementation.cpp
* @brief Vulkan Memory Allocator implementation file
* ------------------------------------------------
* @author Ewan Burnett(EwanBurnettSK@Outlook.com)
* @date 2025/07/29
*/
#if __ANDROID__
#define VMA_STATIC_VULKAN_FUNCTIONS 0 
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#endif
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
