#ifndef DROPKEY_HPP
#define DROPKEY_HPP

#include "EventOutputQueue.hpp"
#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropKey final : public RemapFuncBase {
public:
  DropKey(void) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPKEY),
                  dropKey_(false),
                  dropConsumerKey_(false),
                  dropPointingButton_(false),
                  modifierMatched_(false),
                  dropTargetAutogenId_(0) {}

  bool remap(RemapParams& remapParams) override;
  void cancelEventOutputQueueItems(void) override;

  void add(AddDataType datatype, AddValue newval) override;

  // Always call `remap`.
  bool isActive(bool iskeydown) override {
    if (iskeydown) {
      return true;
    } else {
      return !dropped_.empty();
    }
  }

private:
  void dropKey(EventOutputQueue::Item& item);

  class Item final : public List::Item {
  public:
    Item(const Params_Base& paramsBase) : fromEvent(paramsBase) {}

    FromEvent fromEvent;
  };

  Vector_ModifierFlag fromModifierFlags_;
  bool dropKey_;
  bool dropConsumerKey_;
  bool dropPointingButton_;

  bool modifierMatched_;
  uint64_t dropTargetAutogenId_;
  List dropped_;
};
}
}

#endif
