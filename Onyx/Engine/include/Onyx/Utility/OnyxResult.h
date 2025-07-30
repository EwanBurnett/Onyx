#ifndef __ONYX_RESULT_H
#define __ONYX_RESULT_H

namespace Onyx{
    enum class OnyxResult{
        ONYX_SUCCESS = 0, 
        ONYX_FAILED = -1, 
        ONYX_NOT_IMPLEMENTED = -2, 

        OnyxResult_MAX
    };
}
#endif// __ONYX_RESULT_H
