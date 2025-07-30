#ifndef __BUFFER_H
#define __BUFFER_H

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

#include <cstdint>
#include <shared_mutex>

namespace Onyx{
    namespace Graphics{

        struct BufferCreateInfo{
            const char* name; 
            uint64_t size; 
            VkBufferUsageFlagBits usage;    //TODO: Replace with Onyx internal type...
            VmaAllocationCreateFlagBits flags; 
        };

        struct alignas(16) Buffer{
            //TODO: promote to a class to protect member variables!
            const char* name; 
            uint64_t size; 
            
            //TODO: DX12 Buffer Handles etc.,
            union{
                struct{
                    VkBuffer _buffer;
                    VmaAllocation _alloc; 
                };
            }; 

            std::shared_mutex _Lock;

        };
    }
}
#endif//__BUFFER_H
