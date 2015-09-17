#ifndef IOLOGWRAPPER_HPP
#define IOLOGWRAPPER_HPP

#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "Config.hpp"

#define IOLOG_DEBUG(...)                                    \
  {                                                         \
    if (!org_pqrs_Karabiner::IOLogWrapper::suppressed()) {  \
      if (Config::get_debug()) {                            \
        IOLog("org.pqrs.Karabiner --Debug-- " __VA_ARGS__); \
      }                                                     \
    }                                                       \
  }
#define IOLOG_DEBUG_POINTING(...)                           \
  {                                                         \
    if (!org_pqrs_Karabiner::IOLogWrapper::suppressed()) {  \
      if (Config::get_debug_pointing()) {                   \
        IOLog("org.pqrs.Karabiner --Debug-- " __VA_ARGS__); \
      }                                                     \
    }                                                       \
  }
#define IOLOG_DEVEL(...)                                    \
  {                                                         \
    if (!org_pqrs_Karabiner::IOLogWrapper::suppressed()) {  \
      if (Config::get_debug_devel()) {                      \
        IOLog("org.pqrs.Karabiner --Devel-- " __VA_ARGS__); \
      }                                                     \
    }                                                       \
  }

#define IOLOG_ERROR(...)                                   \
  {                                                        \
    if (!org_pqrs_Karabiner::IOLogWrapper::suppressed()) { \
      IOLog("org.pqrs.Karabiner --Error-- " __VA_ARGS__);  \
    }                                                      \
  }

#define IOLOG_INFO(...)                                    \
  {                                                        \
    if (!org_pqrs_Karabiner::IOLogWrapper::suppressed()) { \
      IOLog("org.pqrs.Karabiner --Info-- " __VA_ARGS__);   \
    }                                                      \
  }

#define IOLOG_WARN(...)                                    \
  {                                                        \
    if (!org_pqrs_Karabiner::IOLogWrapper::suppressed()) { \
      IOLog("org.pqrs.Karabiner --Warn-- " __VA_ARGS__);   \
    }                                                      \
  }

// ------------------------------------------------------------
namespace org_pqrs_Karabiner {
class IOLogWrapper final {
public:
  static bool suppressed(void) { return suppressed_; }
  static void suppress(bool v) { suppressed_ = v; }

  class ScopedSuppress final {
  public:
    ScopedSuppress(void) {
      original = suppressed();
      suppress(true);
    }
    ~ScopedSuppress(void) { suppress(original); }

  private:
    bool original;
  };

private:
  static bool suppressed_;
};
}

#endif
