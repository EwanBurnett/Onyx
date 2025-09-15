#ifndef __COMMAND_BUFFER_H
#define __COMMAND_BUFFER_H

#include <vulkan/vulkan.h>
#include <cstdint>

#include "Fence.h"

namespace Onyx {
    namespace Graphics {

        //TODO: CommandBuffer.h
        enum class ECommandBufferType {
            PRIMARY,
            SECONDARY,
            ECommandBufferType_MAX
        };

        struct CommandBuffer {
            VkCommandBuffer _commandBuffer;

            OnyxResult Begin(const uint32_t flags = 0); 
            OnyxResult End(); 
        };

    }
}
#endif //__COMMAND_BUFFER_H