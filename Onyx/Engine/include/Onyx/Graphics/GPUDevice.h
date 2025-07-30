#ifndef __GPU_DEVICE_H
#define __GPU_DEVICE_H

#include <vector> 
#include "../Utility/OnyxResult.h"
#include "Window.h"

#include "Buffer.h"
#include "Texture.h"

namespace Onyx {
    namespace Graphics {
        class GPUDevice {
        public:
            //virtual ~GPUDevice(); 

            virtual void Init(Window* pWindow) = 0;
            virtual void Shutdown() = 0;

            virtual void CreateSwapchain(Window* pWindow); 

            virtual OnyxResult CreateBuffer(const BufferCreateInfo* pCreateInfo, Buffer* pBuffer);
            virtual void DestroyBuffer(Buffer& buffer); 

            virtual OnyxResult CreateTexture(const TextureCreateInfo* pCreateInfo, Texture* pTexture);
            virtual void DestroyTexture(Texture& buffer); 


        };
    }
}

#endif//__GPU_DEVICE_H
