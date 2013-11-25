#ifndef CORE_HPP
#define CORE_HPP

#include "CallBackWrapper.hpp"

class IOService;
class IONotifier;

namespace org_pqrs_KeyRemap4MacBook {
  namespace Core {
    void start(void);
    void stop(void);

    // ----------------------------------------------------------------------
    bool IOHIKeyboard_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);
    bool IOHIKeyboard_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);

    bool IOHIPointing_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);
    bool IOHIPointing_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);

    // ----------------------------------------------------------------------
    void remap_KeyboardEventCallback(Params_KeyboardEventCallBack& params);
    void remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback& params);
    void remap_RelativePointerEventCallback(Params_RelativePointerEventCallback& params);
    void remap_ScrollWheelEventCallback(Params_ScrollWheelEventCallback& params);
  };
}

#endif
