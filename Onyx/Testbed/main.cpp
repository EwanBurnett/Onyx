#include <cstdio> 
#include <cstdint> 
#include <Onyx/Onyx.h>
#include <Onyx/Version.h>
#include <cstring>

#include <Onyx/Utility/Logger.h>

void Run();

#if __ANDROID__

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


void android_main(android_app* pApp){
    ALOGD("Main Called!\n");

    //Run the application.
    Run();  //TODO: App class /w context
}


#else 

int main() {
    Run();



    return 0; 
}

#endif

void Run(){
    //Initialize the Engine
    Onyx::Initialize();

    uint64_t frameIdx = 0u;
    printf("Running for 100 Frames.\n");

    //TODO: Application Loop
    while (frameIdx < 100u) {   //Prevent the app from running forever...
        //TODO: Poll Events

        //TODO: Update
        printf("Frame %u\r", ++frameIdx);
        Onyx::Utility::Log::Debug("This is a Debug Message!\n");


        //TODO: Render
    }

    //Terminate the Engine
    Onyx::Shutdown();
}