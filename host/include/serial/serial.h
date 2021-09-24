#pragma once

#if defined(OS_WINDOWS)
    #include <windows.h>

#elif defined(OS_LINUX)
    #include <unistd.h>

#endif

/**
 * @brief       Serial wrapper.
 */
class Serial {
  public:
#if defined(OS_WINDOWS)
    using NativeHandle = HANDLE;

#elif defined(OS_LINUX)
    using NativeHandle = int;

#endif
};
