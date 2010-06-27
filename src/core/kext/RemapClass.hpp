#ifndef REMAPCLASS_HPP
#define REMAPCLASS_HPP

#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
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
