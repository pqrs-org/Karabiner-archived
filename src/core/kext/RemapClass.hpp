#ifndef REMAPCLASS_HPP
#define REMAPCLASS_HPP

#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class RemapClass {
  public:
    enum EnableType {
      ENABLE_TYPE_SETKEYBOARDTYPE,
      ENABLE_TYPE_KEY,
      ENABLE_TYPE_CONSUMER,
      ENABLE_TYPE_POINTING,
      ENABLE_TYPE_SIMULTANEOUSKEYPRESSES,
      ENABLE_TYPE_STATUSMESSAGE,
    };

    RemapClass(void) : statusmessage(NULL) {}
    virtual ~RemapClass(void) {}

    virtual void initialize(void) = 0;
    virtual bool enabled(EnableType type) const = 0;

    virtual void remap_setkeyboardtype(KeyboardType& keyboardType) {}
    virtual void remap_key(RemapParams& remapParams) {}
    virtual void remap_consumer(RemapConsumerParams& remapParams) {}
    virtual void remap_pointing(RemapPointingParams_relative& remapParams) {}

    virtual void remap_simultaneouskeypresses(void) {}
    virtual bool handlevirtualkey(const Params_KeyboardEventCallBack& params) { return false; }

    const char* statusmessage;
  };

  namespace RemapClassManager {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);
    void refresh(void);

    // remap_* funtions return the number of enabled entries.
    size_t remap_setkeyboardtype(KeyboardType& keyboardType);
    size_t remap_key(RemapParams& remapParams);
    size_t remap_consumer(RemapConsumerParams& remapParams);
    size_t remap_pointing(RemapPointingParams_relative& remapParams);
    size_t remap_simultaneouskeypresses(void);

    bool handlevirtualkey(const Params_KeyboardEventCallBack& params);
  };
}

#endif
