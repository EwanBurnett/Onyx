#include "../include/Onyx/Onyx.h"
#include "../include/Onyx/Version.h"
#include <cstdio> 

void Onyx::Initialize()
{
    printf("Initializing Onyx v%d.%d.%d [%s] <%s>\n",
        Onyx::Verison::kMajor, Onyx::Verison::kMinor, Onyx::Verison::kIssue, Onyx::Verison::kGitBranch, Onyx::Verison::kGitHash);

    //Testing Compile Definitions
#if ONYX_ENABLE_VULKAN
    printf("Vulkan Enabled!\n");
#endif

#if ONYX_ENABLE_DX12
    printf("DX12 Enabled!\n"); 
#endif
}


void Onyx::Shutdown() {
    printf("Terminating Onyx...\n");
}

int Onyx::GetOne() {
    return 1;
}
