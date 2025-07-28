#ifndef __GPU_DEVICE_H
#define __GPU_DEVICE_H

#include <vector> 
#include "Window.h"

namespace Onyx {
    namespace Graphics {
        class GPUDevice {
        public:
            virtual void Init(Window* pWindow) = 0;
            virtual void Shutdown() = 0;

        };
    }
}

#endif//__GPU_DEVICE_H