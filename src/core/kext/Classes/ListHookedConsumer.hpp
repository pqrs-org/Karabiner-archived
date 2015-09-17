#ifndef LISTHOOKEDCONSUMER_HPP
#define LISTHOOKEDCONSUMER_HPP

#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/hidsystem/IOHIKeyboard.h>
END_IOKIT_INCLUDE;

#include "ListHookedDevice.hpp"

class Params_KeyboardSpecialEventCallback;

namespace org_pqrs_Karabiner {
class ListHookedConsumer final : public ListHookedDevice {
public:
  class Item final : public ListHookedDevice::Item {
  public:
    Item(IOHIDevice* p);
    ~Item(void);

    bool isReplaced(void) const override { return orig_keyboardSpecialEventAction_ != nullptr; }

    void apply(const Params_KeyboardSpecialEventCallback& params);
    void disableNumLock(void);

  private:
    KeyboardSpecialEventCallback orig_keyboardSpecialEventAction_;
    OSObject* orig_keyboardSpecialEventTarget_;

    bool refresh(void) override;

    /** return true if event action is replaced. */
    bool replaceEventAction(void);
    /** return true if event action is replaced. */
    bool restoreEventAction(void);
  };

  static ListHookedConsumer& instance(void);

  bool apply(const Params_KeyboardSpecialEventCallback& params);
  void disableNumLock(void);
};
}

#endif
