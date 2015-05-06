#ifndef DROPKEYAFTERREMAP_HPP
#define DROPKEYAFTERREMAP_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropKeyAfterRemap final : public RemapFuncBase {
public:
  DropKeyAfterRemap(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP, autogenId),
                                           dropped_(false) {}

  void cancelEventOutputQueueItems(EventOutputQueue::Item& item) override;

  // ----------------------------------------
  // [0] => fromKey_
  void add(AddDataType datatype, AddValue newval) override;

private:
  KeyCode key_;
  Flags flags_;
  bool dropped_;
};
}
};

#endif
