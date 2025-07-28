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
        Onyx::Utility::Log::Print("This is a Print Message! %s\n", "hi");
        Onyx::Utility::Log::Print("This is a Print Message!\n");
        Onyx::Utility::Log::Print(Onyx::Utility::ELogColour::BLUE, "This is a Print Message in Blue!\n");
        Onyx::Utility::Log::Debug("This is a Debug Message!\n");
        Onyx::Utility::Log::Message("This is an Info Message!\n");
        Onyx::Utility::Log::Warning("This is a Warning!\n");
        Onyx::Utility::Log::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, "This is an Error Message!\n");
        Onyx::Utility::Log::Fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, "This is a Fatal Error Message!\n");
        Onyx::Utility::Log::Validation("Testing", "This is a Validation Message!\n");

        //TODO: Render
    }

    //Terminate the Engine
    Onyx::Shutdown();
}