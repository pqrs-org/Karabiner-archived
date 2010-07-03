#ifndef BASE_HPP
#define BASE_HPP

#include <mach/mach_types.h>
#define protected public // A hack for access private member of IOHIKeyboard
#define private public
#include <IOKit/hidsystem/IOHIKeyboard.h>
#include <IOKit/hidsystem/IOHIPointing.h>
#undef protected
#undef private

#define IOLOG_DEBUG(...) {                              \
    if (config.debug) {                                 \
      IOLog("KeyRemap4MacBook --Debug-- " __VA_ARGS__); \
    }                                                   \
}
#define IOLOG_DEBUG_POINTING(...) {                     \
    if (config.debug_pointing) {                        \
      IOLog("KeyRemap4MacBook --Debug-- " __VA_ARGS__); \
    }                                                   \
}
#define IOLOG_DEVEL(...) {                              \
    if (config.debug_devel) {                           \
      IOLog("KeyRemap4MacBook --Devel-- " __VA_ARGS__); \
    }                                                   \
}

#define IOLOG_ERROR(...)  IOLog("KeyRemap4MacBook --Error-- " __VA_ARGS__)
#define IOLOG_INFO(...)   IOLog("KeyRemap4MacBook --Info-- "  __VA_ARGS__)
#define IOLOG_WARN(...)   IOLog("KeyRemap4MacBook --Warn-- "  __VA_ARGS__)

#endif
