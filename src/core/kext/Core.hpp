#ifndef CORE_HPP
#define CORE_HPP

#include "Params.hpp"

class IOService;
class IONotifier;

namespace org_pqrs_Karabiner {
namespace Core {
void start(void);
void stop(void);

// ----------------------------------------------------------------------
bool IOHIKeyboard_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);
bool IOHIKeyboard_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);

bool IOHIPointing_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);
bool IOHIPointing_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier);

// ----------------------------------------------------------------------
void remap_KeyboardEventCallback(const Params_Base& paramsBase);
void remap_KeyboardSpecialEventCallback(const Params_Base& paramsBase);
void remap_RelativePointerEventCallback(const Params_Base& paramsBase);
void remap_ScrollWheelEventCallback(const Params_Base& paramsBase);
};
}

#endif
