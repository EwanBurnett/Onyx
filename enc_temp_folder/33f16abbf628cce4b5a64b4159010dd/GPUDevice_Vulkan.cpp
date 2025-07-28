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
        Utility::Log::Debug("Creating Vulkan Instance...\n");

        //Evaluate the requested instance layers
        std::vector<const char*> instanceLayers;
        {
            //Retrieve a list of supported instance layers
            uint32_t layerCount = 0;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            std::vector<VkLayerProperties> layerProps(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data());

        }

        std::vector<const char*> requestedInstanceExtensions;   //TODO: Populate this! 


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
        applicationInfo.pApplicationName = "Application";    //TODO: Populate from the application itself!
        applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);
        applicationInfo.pEngineName = "Onyx-Engine";
        applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, Onyx::Verison::kMajor, Onyx::Verison::kMinor, Onyx::Verison::kIssue);

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pNext = nullptr;     //TODO: Debug Utils!
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

}

void Onyx::Graphics::Vulkan::GPUDevice_Vulkan::Shutdown()
{
    Utility::Log::Message("Destroying Vulkan GPUDevice...\n");
}
