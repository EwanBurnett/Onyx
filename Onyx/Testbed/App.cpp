#include "App.h"

#include <Onyx/Onyx.h>
#include <Onyx/Utility/Logger.h>
#include <Onyx/Graphics/Backends/GPUDevice_Vulkan.h>

#if __ANDROID__

#include <game-activity/GameActivity.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#elif _WIN32 || __LINUX__ 

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
#if _WIN32 || __LINUX__
    glfwPollEvents();
    return !glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(m_Window.GetHandle()));

#elif __ANDROID__

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
#endif
}

void App::Update() {
    //Onyx::Utility::Log::Debug("Update!\n");
}
