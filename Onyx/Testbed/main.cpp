#include <cstdio> 
#include <cstdint> 
#include <Onyx/Onyx.h>
#include <Onyx/Version.h>
#include <cstring>


#include <Onyx/Utility/Logger.h>

#include <Onyx/Graphics/Window.h>
#include <Onyx/Graphics/GPUDevice.h>
#include <Onyx/Graphics/Backends/GPUDevice_Vulkan.h>

#if _WIN32 || __LINUX__
#include <GLFW/glfw3.h>
#elif __ANDROID__
#include <jni.h>
#include <android/log.h>

#include <game-activity/GameActivity.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

extern "C" {
#include <game-activity/native_app_glue/android_native_app_glue.c>
    void android_main(android_app* pApp);
}

#endif

class App {
public:
    App(void* pContext = nullptr) {
        m_Context = pContext;
    }

    void Init() {
        Onyx::Initialize();

        m_Window.Create(1600, 900, m_Context);
        m_Device = new Onyx::Graphics::Vulkan::GPUDevice_Vulkan;
        m_Device->Init();

        m_IsInitialized = true;
    }

    void Shutdown() {
        m_Device->Shutdown();
        delete m_Device;

        m_Window.Destroy();

        //Terminate the Engine
        Onyx::Shutdown();
    }

    void Update() {
        //Onyx::Utility::Log::Debug("Update!\n");
    }

    bool PollEvents() {
#if _WIN32 || __LINUX__

        glfwPollEvents();
        return !glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(m_Window.GetHandle()));

#elif __ANDROID__

        bool done = false;
        while (!done) {
            android_poll_source* pSource = nullptr;
            int events = 0;
            int timeout = 0;
            int r = ALooper_pollOnce(timeout, nullptr, &events,reinterpret_cast<void**>(&pSource));

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

    bool m_IsInitialized = false;
private:
    Onyx::Graphics::Window m_Window;
    Onyx::Graphics::GPUDevice* m_Device;
    void* m_Context;
};



#if __ANDROID__
void handle_cmd(android_app* pApp, int32_t cmd) {
    App* pApplication = reinterpret_cast<App*>(pApp->userData);

    switch (cmd) {
    case APP_CMD_INIT_WINDOW:
        //Initialize the Renderer in the window.
        pApplication->Init();
        break;
    case APP_CMD_TERM_WINDOW:
        //Terminate the renderer.
        pApplication->Shutdown();
        break;
    default:
        break;
    }
}

void android_main(android_app* pApp) {

    //Run the application.
    try {
    App app(pApp);
    pApp->userData = &app;
    pApp->onAppCmd = handle_cmd;


    while (app.PollEvents())
    {
        if (!app.m_IsInitialized) {
            continue;
        }
        app.Update();
    }

    app.Shutdown();


    }
    catch (std::exception& e) {
        Onyx::Utility::Log::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Caught Exception: %s\n", e.what());
    }

}


#else 

int main() {
    App app;
    try {
        app.Init();

        while (app.PollEvents()) {
            if (!app.m_IsInitialized) {
                continue;
            }
            app.Update();
        }

        app.Shutdown();
    }
    catch (std::exception& e) {
        Onyx::Utility::Log::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Caught Exception: %s\n", e.what());
    }

    return 0;
}

#endif
