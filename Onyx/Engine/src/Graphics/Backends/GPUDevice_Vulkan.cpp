#include "../../../include/Onyx/Graphics/Backends/GPUDevice_Vulkan.h"
#include "../../../include/Onyx/Utility/Logger.h"
#include "../../../include/Onyx/Version.h"

#include <vector> 

#if _WIN32 || __LINUX__ 
#include <GLFW/glfw3.h>
#endif

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::Init()
{
    Utility::Log::Message("Creating Vulkan GPUDevice...\n");

    //TODO: For now, do this all inline; Eventually needs to be moved into functions! 

    //Create the Vulkan Instance
    {

        //TODO: All of the following need to be externally populated!
        const bool enableValidationLayers = true;
        const bool enableDebugUtils = true;
        const char* applicationName = "Application";
        const uint32_t applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);

        CreateInstance(enableValidationLayers, enableDebugUtils, applicationName, applicationVersion);

    }

}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::Shutdown()
{
    Utility::Log::Message("Destroying Vulkan GPUDevice...\n");

    DestroyInstance(); 
}


void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::CreateInstance(const bool enableValidationLayers, const bool enableDebugUtils, const char* applicationName, const uint32_t applicationVersion)
{

    Utility::Log::Debug("Creating Vulkan Instance...\n");
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
#elif _WIN32 || __LINUX__ 
        //TODO: Retrieve glfw required instance extensions
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
    Utility::Log::Message("Destroying Vulkan Instance <0x%x>.\n", m_Instance); 
    vkDestroyInstance(m_Instance, nullptr); 
    m_Instance = VK_NULL_HANDLE; 
}



VKAPI_ATTR VkBool32 VKAPI_CALL Onyx::Graphics::Vulkan::GPUDevice_Vulkan::DebugLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    Utility::Log::Validation("Vulkan", "[%d : %s]\n%s\n", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);

    return VK_FALSE;
}

VkResult Onyx::Graphics::Vulkan::GPUDevice_Vulkan::vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
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

