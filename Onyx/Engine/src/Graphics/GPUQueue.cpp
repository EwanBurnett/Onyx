#include "../../include/Onyx/Graphics/GPUQueue.h"
#include "../../include/Onyx/Graphics/GPUDevice.h"
#include <cstring> 

Onyx::Graphics::GPUQueue::GPUQueue(){
    //Zero memory 
    memset(this, 0x00, sizeof(GPUQueue));
}

void Onyx::Graphics::GPUQueue::Create(GPUDevice* pDevice, EQueueType type){
    pDevice->CreateQueue(this, type);
}

void Onyx::Graphics::GPUQueue::Submit(GPUDevice* pDevice, const uint32_t submitCount, SubmitInfo** ppSubmitInfos){
    pDevice->Submit(this, submitCount, ppSubmitInfos);
}

Onyx::Graphics::CommandBuffer Onyx::Graphics::GPUQueue::GetCommandBuffer(GPUDevice* pDevice, ECommandBufferType type){
    return pDevice->AllocateCommandBuffer(this, type);
}


void Onyx::Graphics::GPUQueue::ResetCommandBuffer(GPUDevice* pDevice, CommandBuffer& cmd){
    pDevice->ResetCommandBuffer(this, cmd);
}
         
