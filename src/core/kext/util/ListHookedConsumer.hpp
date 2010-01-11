#ifndef LISTHOOKEDCONSUMER_HPP
#define LISTHOOKEDCONSUMER_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedConsumer : public HookedDevice {
  public:
    HookedConsumer(void) :
      orig_keyboardSpecialEventAction_(NULL),
      orig_keyboardSpecialEventTarget_(NULL) {}

    IOHIKeyboard* get(void) const { return OSDynamicCast(IOHIKeyboard, HookedDevice::get()); }

    KeyboardSpecialEventCallback getOrig_keyboardSpecialEventAction(void) const { return orig_keyboardSpecialEventAction_; }
    OSObject* getOrig_keyboardSpecialEventTarget(void) const { return orig_keyboardSpecialEventTarget_; }

  private:
    KeyboardSpecialEventCallback orig_keyboardSpecialEventAction_;
    OSObject* orig_keyboardSpecialEventTarget_;

    bool initialize(IOHIDevice* d);
    bool refresh(void);
    bool terminate(void);

    /** return true if event action is replaced. */
    bool replaceEventAction(void);
    /** return true if event action is replaced. */
    bool restoreEventAction(void);
  };

  class ListHookedConsumer : public ListHookedDevice {
  public:
    static ListHookedConsumer& instance(void);
    HookedConsumer* get(void) { return static_cast<HookedConsumer*>(ListHookedDevice::get()); }
    HookedConsumer* get(const IOHIKeyboard* kbd) { return static_cast<HookedConsumer*>(ListHookedDevice::get(kbd)); }

  private:
    HookedConsumer item_[MAXNUM];

    HookedDevice* getItem(int index) {
      if (index < 0 || index >= MAXNUM) return NULL;
      return item_ + index;
    }
  };
}

#endif
