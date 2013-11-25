#ifndef IOLOGWRAPPER_HPP
#define IOLOGWRAPPER_HPP

#define IOLOG_DEBUG(...) {                              \
    if (Config::get_debug()) {                          \
      IOLog("KeyRemap4MacBook --Debug-- " __VA_ARGS__); \
    }                                                   \
}
#define IOLOG_DEBUG_POINTING(...) {                     \
    if (Config::get_debug_pointing()) {                 \
      IOLog("KeyRemap4MacBook --Debug-- " __VA_ARGS__); \
    }                                                   \
}
#define IOLOG_DEVEL(...) {                              \
    if (Config::get_debug_devel()) {                    \
      IOLog("KeyRemap4MacBook --Devel-- " __VA_ARGS__); \
    }                                                   \
}

#define IOLOG_ERROR(...)  IOLog("KeyRemap4MacBook --Error-- " __VA_ARGS__)
#define IOLOG_INFO(...)   IOLog("KeyRemap4MacBook --Info-- "  __VA_ARGS__)
#define IOLOG_WARN(...)   IOLog("KeyRemap4MacBook --Warn-- "  __VA_ARGS__)

#endif
