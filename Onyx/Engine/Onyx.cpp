#include "Onyx.h"
#include "Version.h"
#include <cstdio> 

void Onyx::Initialize()
{
    //TODO: Version [Major.Minor.Issue] + Git branch 
    printf("Initializing Onyx v%d.%d.%d [%s]<%s>\n",
        Onyx::Verison::kMajor, Onyx::Verison::kMinor, Onyx::Verison::kIssue, Onyx::Verison::kGitBranch, Onyx::Verison::kGitHash);
}


void Onyx::Shutdown() {
    printf("Terminating Onyx...\n");
}

int Onyx::GetOne() {
    return 1;
}
