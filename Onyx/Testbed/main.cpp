#include <cstdio> 
#include <cstdint> 
#include <Onyx/Onyx.h>
#include <Onyx/Version.h>
#include <cstring>


#include <Onyx/Utility/Logger.h>

#include "App.h"


#if _WIN32 || __linux__

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
    App app(pApp);
    try {
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
        app.Shutdown();
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
        app.Shutdown();
    }

    return 0;
}

#endif
