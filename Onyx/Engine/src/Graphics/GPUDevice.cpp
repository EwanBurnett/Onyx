#include "../../include/Onyx/Graphics/GPUDevice.h"

void Onyx::Graphics::GPUDevice::Init(Window* pWindow) {

}

void Onyx::Graphics::GPUDevice::Shutdown() {

}

void Onyx::Graphics::GPUDevice::CreateSwapchain(Window* pWindow) {

}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::CreateBuffer(const BufferCreateInfo* pCreateInfo, Buffer* pBuffer){
    return OnyxResult::ONYX_NOT_IMPLEMENTED; 
}

void Onyx::Graphics::GPUDevice::DestroyBuffer(Buffer& buffer){

}

Onyx::OnyxResult Onyx::Graphics::GPUDevice::CreateTexture(const TextureCreateInfo* pCreateInfo, Texture* pTexture){
    return OnyxResult::ONYX_NOT_IMPLEMENTED; 
}

void Onyx::Graphics::GPUDevice::DestroyTexture(Texture& buffer){

}

