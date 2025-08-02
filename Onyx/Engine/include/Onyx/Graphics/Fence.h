#ifndef __FENCE_H
#define __FENCE_H

#include <cstdint> 

#include <vulkan/vulkan.h> 
#include "../Utility/OnyxResult.h"

namespace Onyx {
    namespace Graphics {

        class GPUDevice; 

        union FenceHandle {
            VkSemaphore _fence;     //Timeline Semaphore!
            //Microsoft::WRL::ComPtr<ID3D12Fence> _Fence; 
        };

        class Fence {
        public: 
            Fence(); 
            
            void Signal(GPUDevice* pDevice, const uint64_t value); 
            OnyxResult Wait(GPUDevice* pDevice, const uint64_t value, const uint64_t timeout = UINT64_MAX) const;

            FenceHandle& Get(); 
        private: 
            FenceHandle m_Handle; 
        };
    }
}

#endif
