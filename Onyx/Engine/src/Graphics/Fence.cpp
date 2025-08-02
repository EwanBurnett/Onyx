#include "../../include/Onyx/Graphics/Fence.h"
#include "../../include/Onyx/Graphics/GPUDevice.h"
#include <cstring>  //memset

Onyx::Graphics::Fence::Fence()
{
    memset(&m_Handle, 0x00, sizeof(FenceHandle)); //TODO: ZeroMemory utility!
}

void Onyx::Graphics::Fence::Signal(GPUDevice* pDevice, const uint64_t value)
{
    pDevice->SignalFence(*this, value); 
}

Onyx::OnyxResult Onyx::Graphics::Fence::Wait(GPUDevice* pDevice, const uint64_t value, const uint64_t timeout) const
{
    return pDevice->WaitForFences(this, 1, &value, timeout, true); 
}

Onyx::Graphics::FenceHandle& Onyx::Graphics::Fence::Get() {
    return m_Handle; 
}
