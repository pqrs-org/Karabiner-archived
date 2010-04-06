#ifndef CORE_HPP
#define CORE_HPP

#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace Core {
    void initialize(void);
    void terminate(void);
    void start(void);
    void stop(void);

    // ----------------------------------------------------------------------
    bool notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier);
    bool notifierfunc_unhookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier);

    bool notifierfunc_hookPointing(void* target, void* refCon, IOService* newService, IONotifier* notifier);
    bool notifierfunc_unhookPointing(void* target, void* refCon, IOService* newService, IONotifier* notifier);

    // ----------------------------------------------------------------------
    // We use the "passthrough" argument to ignore keys which are remapped by KeyEventInputQueue.
    void remap_KeyboardEventCallback(Params_KeyboardEventCallBack& params, bool passthrough);
    void remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback& params);
    void remap_RelativePointerEventCallback(Params_RelativePointerEventCallback& params);
    void remap_ScrollWheelEventCallback(Params_ScrollWheelEventCallback& params);
  };
}

#endif
