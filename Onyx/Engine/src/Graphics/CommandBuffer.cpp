#include "../../include/Onyx/Graphics/CommandBuffer.h"

Onyx::OnyxResult Onyx::Graphics::CommandBuffer::Begin(const uint32_t flags) {
    //TODO: Dispatch this through the Rendering Engine... 
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.flags = flags;

    VkResult res = vkBeginCommandBuffer(_commandBuffer, &beginInfo);
    if (res != VK_SUCCESS) {
        return OnyxResult::ONYX_FAILED;
    }

    return OnyxResult::ONYX_SUCCESS;
}

Onyx::OnyxResult Onyx::Graphics::CommandBuffer::End() {
    //TODO: Dispatch this through the Rendering Engine... 
    VkResult res = vkEndCommandBuffer(_commandBuffer);
    if (res != VK_SUCCESS) {
        return OnyxResult::ONYX_FAILED;
    }

    return OnyxResult::ONYX_SUCCESS;
}
