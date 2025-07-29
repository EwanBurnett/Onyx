#ifndef __VERSION_H
#define __VERSION_H
/**
* CMAKE GENERATED FILE - DO NOT TOUCH  
* Configures the Engine's Version. 
* -------------------------------------------------
* @author Ewan Burnett(EwanBurnettSK@Outlook.com)
* @date 2025/07/27
*/
#include <cstdint> 

namespace Onyx{
    namespace Verison{ 
        constexpr uint8_t kMajor = 0;
        constexpr uint8_t kMinor = 0;
        constexpr uint8_t kIssue = 1;

        constexpr const char* kGitBranch = u8"dev/gpu-objects";
        constexpr const char* kGitHash = u8"ee067d7";
    }
}

#endif//__VERSION_H
