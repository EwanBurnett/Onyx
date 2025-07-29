#include "../include/Onyx/Onyx.h"
#include "../include/Onyx/Version.h"
#include <../include/Onyx/Utility/Logger.h>
#include <cstdio> 

#if _WIN32 || __LINUX__ 
#include <GLFW/glfw3.h>
#endif

void Onyx::Initialize()
{
    Utility::Log::Message("Initializing Onyx v%d.%d.%d [%s] <%s>\n", Onyx::Verison::kMajor, Onyx::Verison::kMinor, Onyx::Verison::kIssue, Onyx::Verison::kGitBranch, Onyx::Verison::kGitHash);

#if _WIN32 || __LINUX__ 
    glfwInit();
#endif

    //Testing Compile Definitions
#if ONYX_ENABLE_VULKAN
    Utility::Log::Debug("Vulkan Enabled!\n");
#endif

#if ONYX_ENABLE_DX12
    Utility::Log::Debug("DX12 Enabled!\n");
#endif

#if ONYX_ENABLE_IMGUI
    Utility::Log::Debug("ImGui Enabled!\n");
#endif

}


void Onyx::Shutdown() {
    Utility::Log::Message("Terminating Onyx...\n");
#if _WIN32 || __LINUX__ 
    glfwTerminate();
#endif

}

int Onyx::GetOne() {
    return 1;
}
