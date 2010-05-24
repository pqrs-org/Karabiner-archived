#ifndef LISTHOOKEDKEYBOARD_HPP
#define LISTHOOKEDKEYBOARD_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedKeyboard : public HookedDevice {
  public:
    HookedKeyboard(void) :
      orig_keyboardEventAction_(NULL),
      orig_keyboardEventTarget_(NULL),
      orig_updateEventFlagsAction_(NULL),
      orig_updateEventFlagsTarget_(NULL)
    {}

    IOHIKeyboard* get(void) const { return OSDynamicCast(IOHIKeyboard, HookedDevice::get()); }

    KeyboardEventCallback getOrig_keyboardEventAction(void) const { return orig_keyboardEventAction_; }
    OSObject* getOrig_keyboardEventTarget(void) const { return orig_keyboardEventTarget_; }

    UpdateEventFlagsCallback getOrig_updateEventFlagsAction(void) const { return orig_updateEventFlagsAction_; }
    OSObject* getOrig_updateEventFlagsTarget(void) const { return orig_updateEventFlagsTarget_; }

  private:
    KeyboardEventCallback orig_keyboardEventAction_;
    OSObject* orig_keyboardEventTarget_;

    UpdateEventFlagsCallback orig_updateEventFlagsAction_;
    OSObject* orig_updateEventFlagsTarget_;

    AbsoluteTime orig_initialKeyRepeat_;

    bool initialize(IOHIDevice* d);
    bool refresh(void);
    bool terminate(void);
    bool isReplaced(void) const { return orig_keyboardEventAction_ != NULL; }

    /** return true if event action is replaced. */
    bool replaceEventAction(void);
    /** return true if event action is replaced. */
    bool restoreEventAction(void);
  };

  class ListHookedKeyboard : public ListHookedDevice {
  public:
    static ListHookedKeyboard& instance(void);
    HookedKeyboard* get(void) { return static_cast<HookedKeyboard*>(ListHookedDevice::get()); }
    HookedKeyboard* get(const IOHIKeyboard* kbd) { return static_cast<HookedKeyboard*>(ListHookedDevice::get(kbd)); }

    static void hook_KeyboardEventCallback_queued(OSObject* target,
                                                  EventType eventType,
                                                  Flags flags,
                                                  KeyCode key,
                                                  CharCode charCode,
                                                  CharSet charSet,
                                                  OrigCharCode origCharCode,
                                                  OrigCharSet origCharSet,
                                                  KeyboardType keyboardType,
                                                  bool repeat,
                                                  AbsoluteTime ts,
                                                  OSObject* sender,
                                                  void* refcon);

  private:
    HookedKeyboard item_[MAXNUM];

    HookedDevice* getItem(int index) {
      if (index < 0 || index >= MAXNUM) return NULL;
      return item_ + index;
    }
  };
}

#endif
