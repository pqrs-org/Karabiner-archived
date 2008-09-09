#ifndef REMAP_HPP
#define REMAP_HPP

#include "base.hpp"
#include "bridge.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  struct Params_KeyboardEventCallBack {
    OSObject *target;
    unsigned int eventType;
    unsigned int flags;
    unsigned int key;
    unsigned int charCode;
    unsigned int charSet;
    unsigned int origCharCode;
    unsigned int origCharSet;
    unsigned int keyboardType;
    bool repeat;
    AbsoluteTime ts;
    OSObject *sender;
    void *refcon;
  };
  struct Params_KeyboardSpecialEventCallback {
    OSObject *target;
    unsigned int eventType;
    unsigned int flags;
    unsigned int key;
    unsigned int flavor;
    UInt64 guid;
    bool repeat;
    AbsoluteTime ts;
    OSObject *sender;
    void *refcon;
  };
  struct Params_RelativePointerEventCallback {
    OSObject *target;
    int buttons;
    int dx;
    int dy;
    AbsoluteTime ts;
    OSObject *sender;
    void *refcon;
  };

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
    Params_RelativePointerEventCallback *params;
    bool *ex_dropEvent;
  };
  void remap_pointing_relative_core(const RemapPointingParams_relative &remapParams);
}

#endif
