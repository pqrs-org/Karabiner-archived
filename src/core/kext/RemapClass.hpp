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
      ENABLE_TYPE_STATUSMESSAGE,
    };

    RemapClass(void) : statusmessage(NULL) {}
    virtual ~RemapClass(void);

    virtual void initialize(void) = 0;
    virtual bool enabled(EnableType type) const = 0;

    virtual void remap_setkeyboardtype(KeyboardType& keyboardType) {}
    virtual void remap_key(RemapParams& remapParams) {}
    virtual void remap_consumer(RemapConsumerParams& remapParams) {}
    virtual void remap_pointing(RemapPointingParams_relative& remapParams) {}

    const char* statusmessage;
  };

  namespace RemapClassManager {
    void initialize(void);
    void terminate(void);
    void refresh(void);

    void remap_setkeyboardtype(KeyboardType& keyboardType);
    void remap_key(RemapParams& remapParams);
    void remap_consumer(RemapConsumerParams& remapParams);
    void remap_pointing(RemapPointingParams_relative& remapParams);
  };
}

#endif
