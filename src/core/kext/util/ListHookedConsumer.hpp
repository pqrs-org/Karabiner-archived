#ifndef LISTHOOKEDCONSUMER_HPP
#define LISTHOOKEDCONSUMER_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedConsumer : public HookedDevice {
  public:
    HookedConsumer(void);

    bool initialize(IOHIDevice *_device);
    bool refresh(void);
    bool terminate(void);

    IOHIKeyboard *get(void) const { return OSDynamicCast(IOHIKeyboard, HookedDevice::get()); }

    KeyboardSpecialEventCallback getOrig_keyboardSpecialEventAction() const { return orig_keyboardSpecialEventAction; }
    OSObject *getOrig_keyboardSpecialEventTarget() const { return orig_keyboardSpecialEventTarget; }

  private:
    KeyboardSpecialEventCallback orig_keyboardSpecialEventAction;
    OSObject *orig_keyboardSpecialEventTarget;
  };

  class ListHookedConsumer : public ListHookedDevice {
  public:
    static ListHookedConsumer &instance(void);
    HookedConsumer *get(void) { return static_cast<HookedConsumer *>(ListHookedDevice::get()); }
    HookedConsumer *get(const IOHIKeyboard *kbd) { return static_cast<HookedConsumer *>(ListHookedDevice::get(kbd)); }

  private:
    HookedConsumer item[MAXNUM];

    HookedDevice *getItem(int index) {
      if (index < 0 || index >= MAXNUM) return NULL;
      return item + index;
    }
  };
}

#endif
