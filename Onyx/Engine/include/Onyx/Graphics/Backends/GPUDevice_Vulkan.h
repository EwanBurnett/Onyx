#ifndef __GPU_DEVICE_VULKAN_H
#define __GPU_DEVICE_VULKAN_H

#include "../GPUDevice.h"; 
#include "vk_mem_alloc.h"
#include <vulkan/vulkan.h> 

namespace Onyx {
    namespace Graphics {
        namespace Vulkan {
            class GPUDevice_Vulkan : public GPUDevice{
            public: 
                void Init() override; 
                void Shutdown() override; 


            private: 
                void CreateInstance(const bool enableValidationLayers, const bool enableDebugUtils, const char* applicationName, const uint32_t applicationVersion); 
                void DestroyInstance(); 

            private: 
                static VKAPI_ATTR VkBool32 VKAPI_CALL DebugLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
                VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
                VkResult vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger, const VkAllocationCallbacks* pAllocator);



            private: 
                VkInstance m_Instance; 
                VkPhysicalDevice m_PhysicalDevice; 
                VkDevice m_Device; 

            };
        }
    }
}

#endif//__GPU_DEVICE_VULKAN_H