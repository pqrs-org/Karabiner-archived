#ifndef IOLOGWRAPPER_HPP
#define IOLOGWRAPPER_HPP

#include "Config.hpp"

#define IOLOG_DEBUG(...) {                                         \
    if (! org_pqrs_KeyRemap4MacBook::IOLogWrapper::suppressed()) { \
      if (Config::get_debug()) {                                   \
        IOLog("KeyRemap4MacBook --Debug-- " __VA_ARGS__);          \
      }                                                            \
    }                                                              \
}
#define IOLOG_DEBUG_POINTING(...) {                                \
    if (! org_pqrs_KeyRemap4MacBook::IOLogWrapper::suppressed()) { \
      if (Config::get_debug_pointing()) {                          \
        IOLog("KeyRemap4MacBook --Debug-- " __VA_ARGS__);          \
      }                                                            \
    }                                                              \
}
#define IOLOG_DEVEL(...) {                                         \
    if (! org_pqrs_KeyRemap4MacBook::IOLogWrapper::suppressed()) { \
      if (Config::get_debug_devel()) {                             \
        IOLog("KeyRemap4MacBook --Devel-- " __VA_ARGS__);          \
      }                                                            \
    }                                                              \
}

#define IOLOG_ERROR(...) {                                         \
    if (! org_pqrs_KeyRemap4MacBook::IOLogWrapper::suppressed()) { \
      IOLog("KeyRemap4MacBook --Error-- " __VA_ARGS__);            \
    }                                                              \
}

#define IOLOG_INFO(...) {                                          \
    if (! org_pqrs_KeyRemap4MacBook::IOLogWrapper::suppressed()) { \
      IOLog("KeyRemap4MacBook --Info-- "  __VA_ARGS__);            \
    }                                                              \
}

#define IOLOG_WARN(...) {                                          \
    if (! org_pqrs_KeyRemap4MacBook::IOLogWrapper::suppressed()) { \
      IOLog("KeyRemap4MacBook --Warn-- "  __VA_ARGS__);            \
    }                                                              \
}


// ------------------------------------------------------------
namespace org_pqrs_KeyRemap4MacBook {
  class IOLogWrapper {
  public:
    static bool suppressed(void) { return suppressed_; }
    static void suppress(bool v) { suppressed_ = v; }

    class ScopedSuppress {
    public:
      ScopedSuppress(void) { original = suppressed(); suppress(true); }
      ~ScopedSuppress(void) { suppress(original); }

    private:
      bool original;
    };

  private:
    static bool suppressed_;
  };
}

#endif
