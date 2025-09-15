#ifndef __ONYX_RESULT_H
#define __ONYX_RESULT_H

namespace Onyx{
    enum class OnyxResult{
        ONYX_SUCCESS = 1, 
        ONYX_FAILED = -1, 
        ONYX_NOT_IMPLEMENTED = -2, 
        ONYX_TIMEOUT = -3, 
        ONYX_OUT_OF_DATE = -4, 

        OnyxResult_MAX
    };
}
#endif// __ONYX_RESULT_H
