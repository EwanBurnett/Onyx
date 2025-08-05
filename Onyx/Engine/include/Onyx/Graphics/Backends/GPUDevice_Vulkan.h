#ifndef __GPU_DEVICE_VULKAN_H
#define __GPU_DEVICE_VULKAN_H
/*
* @file GPUDevice_Vulkan.h
* @brief Vulkan GPU backend
* ------------------------------------------------
* @author Ewan Burnett(EwanBurnettSK@Outlook.com)
* @date 2025/07/29
*/
#include "../GPUDevice.h" 
#include "vk_mem_alloc.h"
#include <vulkan/vulkan.h> 
#include <unordered_map>

namespace Onyx {
    namespace Graphics {
        namespace Vulkan {
            class GPUDevice_Vulkan : public GPUDevice {
            public:
                GPUDevice_Vulkan();

                void Init(Window* pWindow) override;
                void Shutdown() override;

                OnyxResult Present() override; 

                void Submit(GPUQueue* pQueue, const uint32_t submitCount, SubmitInfo** ppSubmitInfos) override;

                void CreateSwapchain(Window* pWindow) override;

                OnyxResult CreateBuffer(const BufferCreateInfo* pCreateInfo, Buffer* pBuffer) override;
                void DestroyBuffer(Buffer& buffer) override;

                OnyxResult CreateTexture(const TextureCreateInfo* pCreateInfo, Texture* pTexture) override;
                void DestroyTexture(Texture& buffer) override;


                OnyxResult CreateFence(const uint64_t initialValue, Fence* pFence, const char* name) override;
                void DestroyFence(Fence& fence) override;
                OnyxResult SignalFence(Fence& fence, const uint64_t value) override;
                OnyxResult WaitForFences(const Fence* const pFences, const uint32_t count, const uint64_t* pValues, const uint64_t timeout, const bool waitForAll) override;

                OnyxResult CreateQueue(GPUQueue* pQueue, EQueueType type) override; 
                void DestroyQueue(GPUQueue& queue) override;

                CommandBuffer AllocateCommandBuffer(GPUQueue* pQueue, ECommandBufferType type) override;
                void ResetCommandBuffer(GPUQueue* pQueue, CommandBuffer& cmd) override;



            private:
                void CreateInstance(const bool enableValidationLayers, const bool enableDebugUtils, const char* applicationName, const uint32_t applicationVersion);
                void DestroyInstance();

                void SelectPhysicalDevice(const VkPhysicalDeviceFeatures& requiredFeatures = {});

                void CreateDevice(const std::vector<const char*>& requiredExtensions, const VkPhysicalDeviceFeatures& requiredFeatures = {});
                void DestroyDevice();

                void CreateSurface(Window* pWindow);
                void DestroySurface();

                void DestroySwapchain();

                void CreateVkFence(VkFence* pFence, const char* name); 
                void DestroyVkFence(VkFence& fence); 

                void CreateBinarySemaphore(VkSemaphore* semaphore, const char* name); 
                void DestroyBinarySemaphore(VkSemaphore& semaphore); 

                void CreateVMAAllocator();
                void DestroyVMAAllocator();

                void CreateDebugUtilsObjects();
                void DestroyDebugUtilsObjects();

            private:
                static VKAPI_ATTR VkBool32 VKAPI_CALL DebugLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
                VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
                VkResult vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger, const VkAllocationCallbacks* pAllocator);


                VkResult vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo);

            private:
                VkInstance m_Instance;
                VkPhysicalDevice m_PhysicalDevice;
                VkDevice m_Device;
                VkSurfaceKHR m_Surface;

                VkSwapchainKHR m_Swapchain;
                VkFormat m_SwapchainFormat;
                VkExtent2D m_SwapchainExtent;
                std::vector<VkImage> m_SwapchainImages;
                std::vector<VkImageView> m_SwapchainImageViews;
                uint32_t m_SwapchainImageIndex; 

                VkDebugUtilsMessengerEXT m_DebugUtilsMessenger;


                VkFence m_fFrameReady; 
                VkSemaphore m_BinarySemaphore; 

                std::unordered_map<uint32_t, uint32_t> m_AllocatedQueueCounts;
                //TODO: Queue Abstraction + Multiple Queues
                uint32_t m_QueueFamilyIndex;
                VkQueue m_Queue;

                VmaAllocator m_VMAAllocator;

                //TODO: Command Buffer Abstraction + Frames in Flight
                VkCommandPool m_CommandPool; 
                VkCommandBuffer m_CommandBuffer; 

                //TODO: Settings 
                bool m_EnableDebugUtils;
            };
        }
    }
}

#endif//__GPU_DEVICE_VULKAN_H
