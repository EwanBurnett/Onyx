#include "App.h"

#include <Onyx/Onyx.h>
#include <Onyx/Utility/Logger.h>
#include <Onyx/Graphics/Backends/GPUDevice_Vulkan.h>

#if __ANDROID__

#include <game-activity/GameActivity.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#elif _WIN32 || __linux__ 

#include <GLFW/glfw3.h>

#endif

App::App(void* pContext) {
    m_Context = pContext;

    m_Device = nullptr; 
    m_IsInitialized = false; 
}

void App::Init() {
    Onyx::Initialize();

    m_Window.Create(1600, 900, m_Context);
    m_Device = new Onyx::Graphics::Vulkan::GPUDevice_Vulkan;
    m_Device->Init(&m_Window);


    //Testing Resource Creation
    {
        Onyx::Graphics::BufferCreateInfo createInfo = {};
        createInfo.name = "Debug Buffer"; 
        createInfo.size = 1u << 12u; 
        createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;// | VK_BUFFER_USAGE_TRANSFER_DST_BIT; 
        createInfo.flags = {}; 

        Onyx::Graphics::Buffer buffer = {};
        m_Device->CreateBuffer(&createInfo, &buffer); 


        Onyx::Utility::Log::Debug("Sizeof(Buffer) = %d\n", sizeof(Onyx::Graphics::Buffer));
        //Check buffer values
        Onyx::Utility::Log::Debug("Created Buffer!\nName: %s\nSize: %d\nVkBuffer Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", buffer.name, buffer.size, buffer._buffer, buffer._alloc);

        m_Device->DestroyBuffer(buffer); 
        Onyx::Utility::Log::Debug("Destroyed Buffer!\nName: %s\nSize: %d\nVkBuffer Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", buffer.name, buffer.size, buffer._buffer, buffer._alloc);
    }
    {
        Onyx::Utility::Log::Debug("Sizeof(Texture) = %d\n", sizeof(Onyx::Graphics::Texture));
        Onyx::Graphics::TextureCreateInfo createInfo = {}; 
        createInfo.name = "Debug Texture"; 
        createInfo.width = 1024; 
        createInfo.height = 1024;
        createInfo.depth = 1; 
        createInfo.type = Onyx::Graphics::ETextureType::TEXTURE_2D; 
        createInfo.format = VK_FORMAT_R8G8B8A8_SRGB; 
        createInfo.mipLevels = 1; 
        createInfo.sampleCount = Onyx::Graphics::ESampleCount::SAMPLE_COUNT_1; 
        createInfo.tiling = VK_IMAGE_TILING_LINEAR; 
        createInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT; 

        printf("hi!\n");
        Onyx::Graphics::Texture texture = {}; 
        m_Device->CreateTexture(&createInfo, &texture);
        Onyx::Utility::Log::Debug("Created Texture!\nName: %s\nWidth:%d\nHeight: %d\nDepth: %d\nVkImage Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", texture.name, texture.width, texture.height, texture.depth, texture._image, texture._alloc);

        m_Device->DestroyTexture(texture); 
        Onyx::Utility::Log::Debug("Destroyed Texture!\nName: %s\nWidth:%d\nHeight: %d\nDepth: %d\nVkImage Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", texture.name, texture.width, texture.height, texture.depth, texture._image, texture._alloc);

    }

    m_IsInitialized = true;

}

void App::Shutdown() {
    m_Device->Shutdown();
    delete m_Device;

    m_Window.Destroy();

    //Terminate the Engine
    Onyx::Shutdown();
}

bool App::PollEvents() {

#if __ANDROID__

    bool done = false;
    while (!done) {
        android_poll_source* pSource = nullptr;
        int events = 0;
        int timeout = 0;
        int r = ALooper_pollOnce(timeout, nullptr, &events, reinterpret_cast<void**>(&pSource));

        switch (r) {
        case ALOOPER_POLL_TIMEOUT:
            [[clang::fallthrough]];
        case ALOOPER_POLL_WAKE:
            done = true;
            break;
        case ALOOPER_EVENT_ERROR:
            break;
        case ALOOPER_POLL_CALLBACK:
            break;
        default:
            if (pSource) {
                pSource->process((reinterpret_cast<android_app*>(m_Context)), pSource);
            }
        }
    }

    android_app* pApp = reinterpret_cast<android_app*>(m_Context);
    return (!pApp->destroyRequested);
#elif _WIN32 || __linux__
    glfwPollEvents();
    return !glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(m_Window.GetHandle()));

#endif
}

void App::Update() {
    //Onyx::Utility::Log::Debug("Update!\n");
}
