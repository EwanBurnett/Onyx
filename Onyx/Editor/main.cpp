#include <cstdio> 
#include <cstdint> 
#include <Onyx/Onyx.h>
#include <Onyx/Version.h>
#include <cstring>

void Run(); 

int main() {
    Run();

    return 0; 
}

void Run(){
    //Initialize the Engine
    Onyx::Initialize();

    uint64_t frameIdx = 0u;
    printf("Running for 1000000 Frames.\n");

    //TODO: Application Loop
    while (frameIdx < 1000000u) {   //Prevent the app from running forever...
        //TODO: Poll Events

        //TODO: Update
        printf("Frame %u\r", ++frameIdx);


        //TODO: Render
    }

    //Terminate the Engine
    Onyx::Shutdown();
}