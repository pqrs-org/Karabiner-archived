#include "listFireConsumerKey.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  ListFireConsumerKey::fire(OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon)
  {
    for (int i = 0; i < size; ++i) {
      Item &item = list[i];
      unsigned int flavor = item.getKey();
      UInt64 guid = static_cast<UInt64>(-1);

      Params_KeyboardSpecialEventCallback params = {
        target, item.getEventType(), item.getFlags(), item.getKey(),
        flavor, guid, false, ts, sender, refcon,
      };

      RemapUtil::fireConsumer(params);
    }
  }
}
