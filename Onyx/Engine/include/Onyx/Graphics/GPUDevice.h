#ifndef __GPU_DEVICE_H
#define __GPU_DEVICE_H

#include <vector> 

namespace Onyx {
    namespace Graphics {
        class GPUDevice {
        public: 
            virtual void Init() = 0; 
            virtual void Shutdown() = 0; 

        };
    }
}

#endif//__GPU_DEVICE_H