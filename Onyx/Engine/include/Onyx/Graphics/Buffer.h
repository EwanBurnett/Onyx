#ifndef __BUFFER_H
#define __BUFFER_H

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

#include <cstdint>

namespace Onyx{
    namespace Graphics{
        
        struct BufferCreateInfo{
            const char* name; 
            uint64_t size; 
            VkBufferUsageFlagBits usage;    //TODO: Replace with Onyx internal type...
            VmaAllocationCreateFlagBits flags; 
        };

        struct Buffer{
            const char* name; 
            uint64_t size; 
            
            //TODO: DX12 Buffer Handles etc.,
            union{
                struct{
                    VkBuffer _buffer;
                    VmaAllocation _alloc; 
                };
            }; 

        };
    }
}
#endif//__BUFFER_H
