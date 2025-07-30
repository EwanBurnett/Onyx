/*
* @file GPUDevice_Vulkan.cpp
* @brief Vulkan GPU backend
* ------------------------------------------------
* @author Ewan Burnett(EwanBurnettSK@Outlook.com)
* @date 2025/07/29
*/
#include "../../../include/Onyx/Graphics/Backends/GPUDevice_Vulkan.h"
#include "../../../include/Onyx/Utility/Logger.h"
#include "../../../include/Onyx/Version.h"
#include <cstring>

#if _WIN32 || __linux__ 
#include <GLFW/glfw3.h>
#elif __ANDROID__
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <vulkan/vulkan_android.h>
#endif

Onyx::Graphics::Vulkan::GPUDevice_Vulkan::GPUDevice_Vulkan()
{
    m_Instance = VK_NULL_HANDLE; 
    m_PhysicalDevice = VK_NULL_HANDLE; 
    m_Device = VK_NULL_HANDLE; 
    m_Queue = VK_NULL_HANDLE; 
    m_QueueFamilyIndex = -1; 
    m_Surface = VK_NULL_HANDLE; 
    m_Swapchain = VK_NULL_HANDLE;
    m_VMAAllocator = VK_NULL_HANDLE; 
    m_DebugUtilsMessenger = VK_NULL_HANDLE;

    m_EnableDebugUtils = false; 

}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::Init(Window* pWindow)
{
    Utility::Log::Message("Creating Vulkan GPUDevice...\n");

    //Create the Vulkan Instance
    {
        //TODO: All of the following need to be externally populated!
        const bool enableValidationLayers = true;
        const bool enableDebugUtils = true;
        const char* applicationName = "Application";
        const uint32_t applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);

        CreateInstance(enableValidationLayers, enableDebugUtils, applicationName, applicationVersion);
    }

    //Select a Physical Device
    VkPhysicalDeviceFeatures requiredFeatures = {}; //TODO: This should be able to be specified from the application...
    requiredFeatures.fillModeNonSolid = VK_TRUE;

    SelectPhysicalDevice(requiredFeatures);

    //Create the Device
    std::vector<const char*> deviceExtensions = {};

    //TODO: Optional extensions!
    deviceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    deviceExtensions.push_back(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);
    deviceExtensions.push_back(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
    deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    CreateDevice(deviceExtensions, requiredFeatures);

    CreateSurface(pWindow);

    CreateSwapchain(pWindow);

    CreateVMAAllocator();

    CreateDebugUtilsObjects(); 
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::Shutdown()
{
    Utility::Log::Message("Destroying Vulkan GPUDevice...\n");
    vkDeviceWaitIdle(m_Device);

    DestroyDebugUtilsObjects();
    DestroyVMAAllocator();
    DestroySwapchain();
    DestroySurface();
    DestroyDevice();
    DestroyInstance();
}



Onyx::OnyxResult Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateBuffer(const BufferCreateInfo* pCreateInfo, Buffer* pBuffer){

    //Iniitalize the output buffer.
    pBuffer->name = pCreateInfo->name;
    pBuffer->size = pCreateInfo->size; 
    pBuffer->_buffer = VK_NULL_HANDLE; 
    pBuffer->_alloc = VK_NULL_HANDLE; 

    //Create the buffer and its allocation handle. 
    {
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.pNext = nullptr;
        bufferCreateInfo.size = pCreateInfo->size;
        bufferCreateInfo.usage = pCreateInfo->usage;
        bufferCreateInfo.queueFamilyIndexCount = 1;
        bufferCreateInfo.pQueueFamilyIndices = &m_QueueFamilyIndex;     //TODO: Multiple Queue Family Support!
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.flags = pCreateInfo->flags;
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        VkResult res = vmaCreateBuffer(m_VMAAllocator, &bufferCreateInfo, &allocInfo, &pBuffer->_buffer, &pBuffer->_alloc, nullptr);
        if(res != VK_SUCCESS){
            Utility::Log::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Failed to create Buffer %s!\n", pCreateInfo->name);
            return OnyxResult::ONYX_FAILED;//TODO: Failure reason! 
        }
    }

    //Register the buffer's object name 
    //TODO: + tag
    if(m_EnableDebugUtils){
        //Register the VkBuffer Object
        {
            VkDebugUtilsObjectNameInfoEXT nameInfo = {}; 
            nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT; 
            nameInfo.pNext = nullptr; 
            nameInfo.pObjectName = pCreateInfo->name;
            nameInfo.objectHandle = (uint64_t)pBuffer->_buffer; 
            nameInfo.objectType = VK_OBJECT_TYPE_BUFFER;

            VkResult res = vkSetDebugUtilsObjectNameEXT(m_Device, &nameInfo); 
            if(res != VK_SUCCESS){
                Utility::Log::Warning("Unable to Register Buffer Name %s!\n", pCreateInfo->name);
            }
        }

        //Set the VMA Allocation's name as well. 
        vmaSetAllocationName(m_VMAAllocator, pBuffer->_alloc, pCreateInfo->name); 
    }


    return OnyxResult::ONYX_SUCCESS; 
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DestroyBuffer(Buffer& buffer){
    //Destroy the buffer via VMA. 
    vmaDestroyBuffer(m_VMAAllocator, buffer._buffer, buffer._alloc); 

    //Invalidate the buffer's data. 
    buffer.name = "NULL";
    buffer.size = -1; 
    buffer._alloc = VK_NULL_HANDLE;
    buffer._buffer = VK_NULL_HANDLE; 
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateInstance(const bool enableValidationLayers, const bool enableDebugUtils, const char* applicationName, const uint32_t applicationVersion)
{
    Utility::Log::Debug("Creating Vulkan Instance...\n");

    m_EnableDebugUtils = enableValidationLayers; 
    
    std::vector<const char*> requestedInstanceLayers;
    std::vector<const char*> requestedInstanceExtensions;

    if (enableValidationLayers) {
        requestedInstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
    }

    if (enableDebugUtils) {
        requestedInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    //Evaluate the requested instance layers
    std::vector<const char*> instanceLayers;
    {
        //Retrieve a list of supported instance layers
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> layerProps(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data());

        for (auto& requestedLayer : requestedInstanceLayers) {
            bool layerFound = false;

            for (auto& layer : layerProps) {
                if (strcmp(requestedLayer, layer.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (layerFound) {
                Utility::Log::Validation("Vulkan", "Loading Instance Layer %s.\n", requestedLayer);
                instanceLayers.push_back(requestedLayer);
            }
            else {
                Utility::Log::Warning("Unable to load Instance Layer %s!\n", requestedLayer);
            }
        }
    }

    //Retrieve platform-specific instance extensions. 
    {
#if __ANDROID__
        requestedInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        requestedInstanceExtensions.push_back("VK_KHR_android_surface");
#elif _WIN32 || __linux__ 
        //Retrieve glfw required instance extensions
        uint32_t glfwExtensionCount = 0u;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            requestedInstanceExtensions.push_back(glfwExtensions[i]);
        }
#endif
    }

    //Evaluate the requested instance extensions 
    std::vector<const char*> instanceExtensions;
    {
        //Retrieve a list of supported instance extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensionProps(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProps.data());

        for (auto& requestedExtension : requestedInstanceExtensions) {
            bool extensionFound = false;

            for (auto& ext : extensionProps) {

                //If we find the requested extension in the list of supported extensions, enable it. 
                if (strcmp(requestedExtension, ext.extensionName) == 0) {
                    extensionFound = true;
                    break;
                }

            }
            if (extensionFound) {
                Utility::Log::Validation("Vulkan", "Loading Instance Extension %s.\n", requestedExtension);
                instanceExtensions.push_back(requestedExtension);
            }
            else {
                Utility::Log::Warning("Unable to load Instance Extension %s!\n", requestedExtension);
            }
        }

    }

    //Populate the Application Info. 
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = applicationName;
    applicationInfo.applicationVersion = applicationVersion;
    applicationInfo.pEngineName = "Onyx-Engine";
    applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, Onyx::Verison::kMajor, Onyx::Verison::kMinor, Onyx::Verison::kIssue);
    applicationInfo.apiVersion = VK_API_VERSION_1_2;

    //Optionally create a Debug messenger
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
    debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugMessengerCreateInfo.pNext = nullptr;
    debugMessengerCreateInfo.pUserData = this;
    debugMessengerCreateInfo.pfnUserCallback = DebugLogCallback;
    debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    //Enable DebugPRINTF
    VkValidationFeatureEnableEXT debugPrintFFeature = VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT;
    VkValidationFeaturesEXT features = {};
    features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    features.pNext = &debugMessengerCreateInfo;
    features.enabledValidationFeatureCount = 1;
    features.pEnabledValidationFeatures = &debugPrintFFeature;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = enableDebugUtils ? &features : nullptr;
    createInfo.flags = 0;
    createInfo.enabledExtensionCount = instanceExtensions.size();
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();
    createInfo.enabledLayerCount = instanceLayers.size();
    createInfo.ppEnabledLayerNames = instanceLayers.data();
    createInfo.pApplicationInfo = &applicationInfo; 

    //Create the VkInstance
    VkResult res = vkCreateInstance(&createInfo, nullptr, &m_Instance);
    if (res != VK_SUCCESS) {
        Utility::Log::Fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Failed to Create Vulkan Instance!\n");
        throw std::runtime_error("Failed to Create Vulkan Instance!\n");
    }

    Utility::Log::Debug("Vulkan Instance Created! -> <0x%x>\n", m_Instance);

}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DestroyInstance()
{
    Utility::Log::Debug("Destroying Vulkan Instance <0x%x>.\n", m_Instance);
    vkDestroyInstance(m_Instance, nullptr);
    m_Instance = VK_NULL_HANDLE;
}


void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::SelectPhysicalDevice(const VkPhysicalDeviceFeatures& requiredFeatures)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, physicalDevices.data());

    const auto& lastDevice = physicalDevices.end();
    for (const auto& device : physicalDevices) {

        //Evaluate device feature compatibility
        VkPhysicalDeviceFeatures deviceFeatures = {};
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        bool featuresValid = true;

        for (size_t i = 0; i < sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32); i++) {
            VkBool32* a = ((VkBool32*)&requiredFeatures) + i;
            VkBool32* b = ((VkBool32*)&deviceFeatures) + i;

            if (*a == VK_TRUE) {
                if (*b != VK_TRUE) {
                    featuresValid = false;
                }
            }
        }

        if (!featuresValid) {
            Utility::Log::Warning("Not all required Physical Device Features were available!\n");
            continue;
        }

        //Prefer a Discrete GPU. 
        VkPhysicalDeviceProperties deviceProperties = {};
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        Utility::Log::Debug("Found Compatible Device %s!\n", deviceProperties.deviceName);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            m_PhysicalDevice = device;
            break;
        }
        else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            m_PhysicalDevice = device;
            break;
        }

        //Continue searching
        else {
            m_PhysicalDevice = device;
            continue;
        }

    }

    if (m_PhysicalDevice == VK_NULL_HANDLE) {
        Utility::Log::Fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, "No Suitable Physical Device found!\n");
        return;
    }

    VkPhysicalDeviceProperties deviceProperties = {};
    vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);

    Utility::Log::Debug("Selected Physical Device: [%s]\n", deviceProperties.deviceName);
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateDevice(const std::vector<const char*>& requiredExtensions, const VkPhysicalDeviceFeatures& requiredFeatures)
{
    Utility::Log::Debug("Creating Vulkan Device.\n");

    std::vector<const char*> deviceExtensions;

    //Validate requested device extensions
    {
        uint32_t propertyCount = 0;
        vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &propertyCount, nullptr);
        std::vector<VkExtensionProperties> extensionProperties(propertyCount);
        vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &propertyCount, extensionProperties.data());

        for (const auto& extension : requiredExtensions) {

            //Evaluate Instance Extension Support
            bool extensionSupported = false;

            for (const auto& property : extensionProperties) {
                if (strcmp(property.extensionName, extension) == 0) {
                    extensionSupported = true;
                    break;
                }
            }

            //Add the extension to our internal list, if supported. 
            if (extensionSupported) {
                Utility::Log::Debug("Enabling Device Extension <%s>.\n", extension);
                deviceExtensions.push_back(extension);
            }
            else {
                Utility::Log::Warning("Extension <%s> is not supported by the current Vulkan Device!\n", extension);
            }
        }
    }

    auto FindGraphicsQueueFamilyIndex = [](VkPhysicalDevice physicalDevice) -> uint32_t {

        uint32_t propCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, nullptr);
        std::vector<VkQueueFamilyProperties> properties(propCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, properties.data());

        uint32_t index = 0;
        for (const auto& p : properties) {

            if (p.queueFlags & VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT) {
                return index;
            }
            index++;
        }

        return 0;
        };

    m_QueueFamilyIndex = FindGraphicsQueueFamilyIndex(m_PhysicalDevice);

    //Enable device features
    VkPhysicalDeviceVulkan12Features vulkan1_2features = {};
    vulkan1_2features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    vulkan1_2features.pNext = nullptr;
    vulkan1_2features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    vulkan1_2features.bufferDeviceAddress = VK_TRUE;
    vulkan1_2features.descriptorIndexing = VK_TRUE;

    VkPhysicalDeviceFeatures2 deviceFeatures = {};
    deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    deviceFeatures.pNext = &vulkan1_2features;
    deviceFeatures.features = requiredFeatures;

    //We want one Graphics queue
    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.queueFamilyIndex = m_QueueFamilyIndex;
    queueCreateInfo.pQueuePriorities = &priority;


    //Create the Device
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = (void*)&deviceFeatures;     //TODO: Support optional featuresets!
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.pEnabledFeatures = nullptr;// &m_pInitInfo->requiredFeatures;

    vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device);
    Utility::Log::Debug("Vulkan Device Created! -> <0x%x>\n", m_Device);

    vkGetDeviceQueue(m_Device, m_QueueFamilyIndex, 0, &m_Queue);

}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DestroyDevice()
{
    Utility::Log::Debug("Destroying Vulkan device <0x%x>\n", m_Device);
    vkDestroyDevice(m_Device, nullptr);
    m_Device = VK_NULL_HANDLE;
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateSurface(Window* pWindow)
{
    VkResult res = VK_SUCCESS;
#if _WIN32 || __linux__
    res = glfwCreateWindowSurface(m_Instance, reinterpret_cast<GLFWwindow*>(pWindow->GetHandle()), nullptr, &m_Surface);
#elif __ANDROID__
    VkAndroidSurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.window = reinterpret_cast<android_app*>(pWindow->GetHandle())->window;

    res = vkCreateAndroidSurfaceKHR(m_Instance, &createInfo, nullptr, &m_Surface);
#endif

    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan Surface!\n");
    }

    Utility::Log::Debug("Creating Vulkan Surface <0x%x>\n", m_Surface);
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DestroySurface()
{
    Utility::Log::Debug("Destroying Vulkan Surface <0x%x>\n", m_Surface);
    vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    m_Surface = VK_NULL_HANDLE;
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateSwapchain(Window* pWindow)
{
    m_SwapchainExtent.width = pWindow->GetWidth();
    m_SwapchainExtent.height = pWindow->GetHeight();

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, m_Surface, &capabilities);

    uint32_t numFormats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &numFormats, nullptr);
    if (numFormats != 0) {
        formats.resize(numFormats);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &numFormats, formats.data());
    }
    //TODO: We're being lazy and just selecting the first swapchain image format. 
    VkSurfaceFormatKHR format = formats[0];
    m_SwapchainFormat = format.format; 


    uint32_t numPresentModes = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &numPresentModes, nullptr);
    if (numPresentModes != 0) {
        presentModes.resize(numPresentModes);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &numPresentModes, presentModes.data());
    }

    VkPresentModeKHR presentMode = [](std::vector<VkPresentModeKHR> presentModes) {

        for (const auto& mode : presentModes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return mode;
            }
            else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                return mode;
            }
            else if (mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
                return mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;    //Fall back to FIFO. 

        }(presentModes);
    
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) 
    {
        imageCount = capabilities.maxImageCount;
    }


    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.oldSwapchain = m_Swapchain;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.queueFamilyIndexCount = 1;
    swapchainCreateInfo.pQueueFamilyIndices = &m_QueueFamilyIndex;
    swapchainCreateInfo.imageExtent.width = pWindow->GetWidth();
    swapchainCreateInfo.imageExtent.height = pWindow->GetHeight();
    swapchainCreateInfo.surface = m_Surface;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;    //TODO: Ensure that this is present in surface capabilities!
    swapchainCreateInfo.clipped = VK_FALSE;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.imageFormat = format.format;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //| VK_IMAGE_USAGE_STORAGE_BIT;// VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageColorSpace = format.colorSpace;
    swapchainCreateInfo.imageArrayLayers = 1;

    auto r = vkCreateSwapchainKHR(m_Device, &swapchainCreateInfo, nullptr, &m_Swapchain);
    if (r != VK_SUCCESS) {
        Utility::Log::Warning("VkResult was %d!\n", r);
        throw std::runtime_error("Failed to create Swapchain!\n");
    }
    Utility::Log::Debug("Creating Vulkan Swapchain <0x%x>\n", m_Swapchain);

    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, nullptr);
    m_SwapchainImages.resize(swapchainImageCount);
    vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, m_SwapchainImages.data());

    for (const auto& img : m_SwapchainImages) {
        VkImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.pNext = nullptr;
        imageViewInfo.flags = 0;
        imageViewInfo.format = swapchainCreateInfo.imageFormat;
        imageViewInfo.image = img;
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.layerCount = 1;
        imageViewInfo.subresourceRange.levelCount = 1;

        VkImageView view = VK_NULL_HANDLE;
        vkCreateImageView(m_Device, &imageViewInfo, nullptr, &view);

        m_SwapchainImageViews.push_back(view);
    }

}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DestroySwapchain()
{
    Utility::Log::Debug("Destroying Vulkan Swapchain <0x%x>\n", m_Swapchain);
    vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
    m_Swapchain = VK_NULL_HANDLE;

    //Destroy the image views too
    for(VkImageView& view : m_SwapchainImageViews){
        vkDestroyImageView(m_Device, view, nullptr);
    }
    m_SwapchainImageViews.clear();
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateVMAAllocator()
{
    VmaVulkanFunctions vulkanFuncs{};
    vulkanFuncs.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkanFuncs.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo createInfo = {};
    createInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    createInfo.instance = m_Instance;
    createInfo.device = m_Device;
    createInfo.pAllocationCallbacks = nullptr;
    createInfo.pDeviceMemoryCallbacks = nullptr;
    createInfo.pHeapSizeLimit = nullptr;
    createInfo.physicalDevice = m_PhysicalDevice;
    createInfo.pVulkanFunctions = &vulkanFuncs;

    vmaCreateAllocator(&createInfo, &m_VMAAllocator);
    Utility::Log::Debug("Creating VMA Allocator <0x%x>\n", m_VMAAllocator);
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DestroyVMAAllocator()
{
    Utility::Log::Debug("Destroying VMA Allocator <0x%x>\n", m_VMAAllocator);
    vmaDestroyAllocator(m_VMAAllocator);
    m_VMAAllocator = VK_NULL_HANDLE;
}


void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateDebugUtilsObjects()
{
    if(m_EnableDebugUtils){
        Utility::Log::Debug("Creating Debug Utils Objects...\n"); 

        //Create the DebugUtilsMessengerEXT object
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {}; 
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; 
        createInfo.pNext = nullptr; 
        createInfo.flags = 0; 
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT; // | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; // | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        createInfo.pfnUserCallback = DebugLogCallback; 


        VkResult res = GPUDevice_Vulkan::vkCreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugUtilsMessenger);
        if(res != VK_SUCCESS){
            Utility::Log::Warning("Failed to create Debug Utils Objects!\n"); 
        }
    }
}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DestroyDebugUtilsObjects()
{
    if(m_EnableDebugUtils){
        Utility::Log::Debug("Destroying Debug Utils Objects...\n");

       GPUDevice_Vulkan::vkDestroyDebugUtilsMessengerEXT(m_Instance, &m_DebugUtilsMessenger, nullptr);
       m_DebugUtilsMessenger = VK_NULL_HANDLE;
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DebugLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    Utility::Log::Validation("Vulkan", "[%d : %s]\n%s\n", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);

    return VK_FALSE;
}

VkResult Onyx::Graphics::Vulkan::GPUDevice_Vulkan::vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkResult Onyx::Graphics::Vulkan::GPUDevice_Vulkan::vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, *pDebugMessenger, pAllocator);
        return VK_SUCCESS;
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


VkResult Onyx::Graphics::Vulkan::GPUDevice_Vulkan::vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo){
    auto func = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(m_Instance, "vkSetDebugUtilsObjectNameEXT"); 
    if(func != nullptr) {
        return func(device, pNameInfo);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

