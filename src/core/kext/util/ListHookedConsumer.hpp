#ifndef LISTHOOKEDCONSUMER_HPP
#define LISTHOOKEDCONSUMER_HPP

#include "base.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedConsumer : public ListHookedDevice {
  public:
    class Item : public ListHookedDevice::Item {
    public:
      Item(IOHIDevice* p);
      ~Item(void);

      bool isReplaced(void) const { return orig_keyboardSpecialEventAction_ != NULL; }

      void apply(const Params_KeyboardSpecialEventCallback& params);
      void disableNumLock(void);

    private:
      KeyboardSpecialEventCallback orig_keyboardSpecialEventAction_;
      OSObject* orig_keyboardSpecialEventTarget_;

      IOLock* replacerestore_lock_;

      bool refresh_callback(void);

      /** return true if event action is replaced. */
      bool replaceEventAction(void);
      /** return true if event action is replaced. */
      bool restoreEventAction(void);
    };

    static ListHookedConsumer& instance(void);

    void apply(const Params_KeyboardSpecialEventCallback& params);
    void disableNumLock(void);
  };
}

#endif
