#ifndef REMAP_HPP
#define REMAP_HPP

#include "base.hpp"
#include "bridge.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  struct RemapParams {
    unsigned int *eventType;
    unsigned int *flags;
    unsigned int *key;
    unsigned int *charCode;
    unsigned int *charSet;
    unsigned int *origCharCode;
    unsigned int *origCharSet;
    unsigned int *keyboardType;
    // Note: the repeat key code is dropped in the core code.
    AbsoluteTime *ts;
    unsigned int ex_origKey;
    KeyRemap4MacBook_bridge::ActiveApplicationInfo::ApplicationType appType;
    ExtraRepeatFunc::ExtraRepeatFunc *ex_extraRepeatFunc;
    unsigned int *ex_extraRepeatFlags;
    unsigned int ex_extraRepeatCounter;
  };
  void remap_core(const RemapParams &params);

  // --------------------
  struct RemapConsumerParams {
    unsigned int *eventType;
    unsigned int *flags;
    unsigned int *key;
    unsigned int *flavor;
    AbsoluteTime *ts;
    KeyCode::KeyCode *ex_remapKeyCode;
  };
  void remap_consumer(const RemapConsumerParams &params);

  // --------------------
  struct RemapPointingParams_relative {
    int *buttons;
    int *dx;
    int *dy;
    AbsoluteTime *ts;
    bool *ex_dropEvent;
  };
  void remap_pointing_relative_core(const RemapPointingParams_relative &params);
}

#endif
