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
                VkInstance m_Instance; 
                VkPhysicalDevice m_PhysicalDevice; 
                VkDevice m_Device; 

            };
        }
    }
}

#endif//__GPU_DEVICE_VULKAN_H