#ifndef __GPU_QUEUE_H
#define __GPU_QUEUE_H

#include <vulkan/vulkan.h>
#include <cstdint>

#include "Fence.h"

namespace Onyx{
    namespace Graphics{

        //TODO: CommandBuffer.h
        enum class ECommandBufferType{
            //TODO: Populate command buffer types!
            ECommandBufferType_MAX
        };

        struct CommandBuffer{
            VkCommandBuffer _commandBuffer;
        };
        
        struct SubmitInfo{
            uint32_t commandBufferCount; 
            CommandBuffer* pCommandBuffers;
            uint32_t waitFenceCount; 
            uint64_t* pWaitValues; 
            Fence* pWaitFences; 
            VkPipelineStageFlags* pWaitDstStageMask;     //TODO: Abstract me!
            uint32_t signalFenceCount; 
            uint64_t* pSignalValues; 
            Fence* pSignalFences; 
        };

        enum class EQueueType{
            QUEUE_TYPE_GRAPHICS, 
            QUEUE_TYPE_TRANSFER, 
            QUEUE_TYPE_COMPUTE,
            EQueueType_MAX
        };

        class GPUQueue{
        public:
            GPUQueue(); 

            //TODO: Queue Names...
            void Create(GPUDevice* pDevice, EQueueType type); 
            void Submit(GPUDevice* pDevice, const uint32_t submitCount, SubmitInfo** ppSubmitInfos); 

            CommandBuffer GetCommandBuffer(GPUDevice* pDevice, ECommandBufferType type); 
            void ResetCommandBuffer(GPUDevice* pDevice, CommandBuffer& cmd); 
            //void ResetCommandPool(GPUDevice* pDevice, )//TODO: CommandPool.h
            
            union{
                struct{
                    VkQueue _queue; 
                    uint32_t _queueFamilyIndex;
                    VkCommandPool _commandPool; 
                };
            };
                
        };
    }
}
#endif// __GPU_QUEUE_H

