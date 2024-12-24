#pragma once

// Instead of defining a wrapper class or smth,
// we can just conditionally include and
// set the following as the "common"
// depending on the platform we are on.

#if defined(_WIN32) || defined(_WIN64)
    #include "../windows/NamedPipeWindowsImpl.h"

    #define NAMED_PIPE NamedPipeWindowsImpl
#else
    #include "../linux/NamedPipeLinuxImpl.h"

    #define NAMED_PIPE NamedPipeLinuxImpl
#endif