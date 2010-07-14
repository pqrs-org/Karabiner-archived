#ifndef REMAPCLASS_HPP
#define REMAPCLASS_HPP

#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapClassManager {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);
    void refresh(void);

    void remap_setkeyboardtype(KeyboardType& keyboardType);
    void remap_key(RemapParams& remapParams);
    void remap_consumer(RemapConsumerParams& remapParams);
    void remap_pointing(RemapPointingParams_relative& remapParams);
    void remap_simultaneouskeypresses(void);

    // return true if dropped.
    bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params);

    bool isEventInputQueueDelayEnabled(void);
  };
}

#endif
