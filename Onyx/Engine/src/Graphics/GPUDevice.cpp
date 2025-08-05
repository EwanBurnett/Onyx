#include "../../include/Onyx/Graphics/GPUDevice.h"

void Onyx::Graphics::GPUDevice::Init(Window* pWindow) {

}

void Onyx::Graphics::GPUDevice::Shutdown() {

}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::Present() {
    return OnyxResult::ONYX_NOT_IMPLEMENTED; 
}

void Onyx::Graphics::GPUDevice::CreateSwapchain(Window* pWindow) {

}
            
void Onyx::Graphics::GPUDevice::Submit(GPUQueue* pQueue, const uint32_t submitCount, SubmitInfo** ppSubmitInfos){

}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::CreateBuffer(const BufferCreateInfo* pCreateInfo, Buffer* pBuffer) {
    return OnyxResult::ONYX_NOT_IMPLEMENTED;
}

void Onyx::Graphics::GPUDevice::DestroyBuffer(Buffer& buffer) {

}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::CreateTexture(const TextureCreateInfo* pCreateInfo, Texture* pTexture) {
    return OnyxResult::ONYX_NOT_IMPLEMENTED;
}

void Onyx::Graphics::GPUDevice::DestroyTexture(Texture& buffer) {

}


Onyx::OnyxResult Onyx::Graphics::GPUDevice::CreateFence(const uint64_t initialValue, Fence* pFence, const char* name)
{
    return OnyxResult::ONYX_NOT_IMPLEMENTED; 
}

void Onyx::Graphics::GPUDevice::DestroyFence(Fence& fence)
{
}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::SignalFence(Fence& fence, const uint64_t value)
{
    return OnyxResult::ONYX_NOT_IMPLEMENTED; 
}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::WaitForFences(const Fence* const pFences, const uint32_t count, const uint64_t* pValues, const uint64_t timeout, const bool waitForAll)
{
    return OnyxResult::ONYX_NOT_IMPLEMENTED; 
}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::CreateQueue(GPUQueue* pQueue, EQueueType type){
    return OnyxResult::ONYX_NOT_IMPLEMENTED; 
}

void Onyx::Graphics::GPUDevice::DestroyQueue(GPUQueue& queue){

}

Onyx::Graphics::CommandBuffer Onyx::Graphics::GPUDevice::AllocateCommandBuffer(GPUQueue* pQueue, ECommandBufferType type){
    return {}; 
}

void Onyx::Graphics::GPUDevice::ResetCommandBuffer(GPUQueue* pQueue, CommandBuffer& cmd){

}



