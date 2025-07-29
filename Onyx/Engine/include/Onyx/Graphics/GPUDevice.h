#ifndef __GPU_DEVICE_H
#define __GPU_DEVICE_H

#include <vector> 
#include "Window.h"

namespace Onyx {
    namespace Graphics {
        class GPUDevice {
        public:
            virtual ~GPUDevice(); 

            virtual void Init(Window* pWindow) = 0;
            virtual void Shutdown() = 0;

            virtual void CreateSwapchain(Window* pWindow); 
        };
    }
}

#endif//__GPU_DEVICE_H