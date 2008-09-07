#include "listFireConsumerKey.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  ListFireConsumerKey::fire(KeyboardSpecialEventCallback callback,
                            OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    if (callback == NULL) return;

    for (int i = 0; i < size; ++i) {
      Item &item = list[i];
      unsigned int flavor = item.getKey();
      unsigned int guid = -1;
      RemapUtil::fireConsumer(callback,
                              target, item.getEventType(), item.getFlags(), item.getKey(),
                              flavor, guid, false, ts, sender, refcon);
    }
  }
}
