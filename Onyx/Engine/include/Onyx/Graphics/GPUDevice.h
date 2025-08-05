#ifndef __GPU_DEVICE_H
#define __GPU_DEVICE_H

#include <vector> 
#include "../Utility/OnyxResult.h"
#include "Window.h"

#include "Buffer.h"
#include "Texture.h"
#include "Fence.h"
#include "GPUQueue.h"

namespace Onyx {
    namespace Graphics {
        class GPUDevice {
        public:
            //virtual ~GPUDevice(); 

            virtual void Init(Window* pWindow) = 0;
            virtual void Shutdown() = 0;

            virtual OnyxResult Present(); 


            virtual void Submit(GPUQueue* pQueue, const uint32_t submitCount, SubmitInfo** ppSubmitInfos); 

            virtual void CreateSwapchain(Window* pWindow); 

            virtual OnyxResult CreateBuffer(const BufferCreateInfo* pCreateInfo, Buffer* pBuffer);
            virtual void DestroyBuffer(Buffer& buffer); 

            virtual OnyxResult CreateTexture(const TextureCreateInfo* pCreateInfo, Texture* pTexture);
            virtual void DestroyTexture(Texture& buffer); 

            virtual OnyxResult CreateFence(const uint64_t initialValue, Fence* pFence, const char* name = nullptr) ;
            virtual void DestroyFence(Fence& fence); 
            virtual OnyxResult SignalFence(Fence& fence, const uint64_t value) ;
            virtual OnyxResult WaitForFences(const Fence* const pFences, const uint32_t count, const uint64_t* pValues, const uint64_t timeout, const bool waitForAll = true) ; 

            virtual OnyxResult CreateQueue(GPUQueue* pQueue, EQueueType type);
            virtual void DestroyQueue(GPUQueue& queue);

            virtual CommandBuffer AllocateCommandBuffer(GPUQueue* pQueue, ECommandBufferType type);
            virtual void ResetCommandBuffer(GPUQueue* pQueue, CommandBuffer& cmd);

        };
    }
}

#endif//__GPU_DEVICE_H
