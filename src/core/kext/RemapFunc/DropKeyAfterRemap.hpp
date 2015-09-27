#ifndef DROPKEYAFTERREMAP_HPP
#define DROPKEYAFTERREMAP_HPP

#include "RemapFuncBase.hpp"
#include "CancelEventOutputQueueItemsHelper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropKeyAfterRemap final : public RemapFuncBase {
public:
  DropKeyAfterRemap(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP, autogenId),
                                           strictFlagsMatch_(false),
                                           dropped_(false) {}

  bool remap(RemapParams& remapParams) override;
  void cancelEventOutputQueueItems(EventOutputQueue::Item& item) override;

  // ----------------------------------------
  // [0] => fromKey_
  void add(AddDataType datatype, AddValue newval) override;

private:
  CancelEventOutputQueueItemsHelper helper_;

  KeyCode key_;
  Flags flags_;
  bool strictFlagsMatch_;
  bool dropped_;
};
}
};

#endif
