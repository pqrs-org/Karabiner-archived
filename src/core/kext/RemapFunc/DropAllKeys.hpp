#ifndef DROPALLKEYS_HPP
#define DROPALLKEYS_HPP

#include "EventOutputQueue.hpp"
#include "RemapFuncBase.hpp"
#include "CancelEventOutputQueueItemsHelper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropAllKeys final : public RemapFuncBase {
public:
  DropAllKeys(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPALLKEYS, autogenId),
                                     dropKey_(false),
                                     dropConsumerKey_(false),
                                     dropPointingButton_(false),
                                     modifierMatched_(false),
                                     doCancel_(false) {}

  bool remap(RemapParams& remapParams) override;
  void cancelEventOutputQueueItems(EventOutputQueue::Item& item) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  void dropKey(EventOutputQueue::Item& item);

  class Item final : public List::Item {
  public:
    Item(const Params_Base& paramsBase) : fromEvent(paramsBase) {}

    FromEvent fromEvent;
  };

  CancelEventOutputQueueItemsHelper helper_;

  Vector_ModifierFlag fromModifierFlags_;
  bool dropKey_;
  bool dropConsumerKey_;
  bool dropPointingButton_;

  bool modifierMatched_;
  bool doCancel_;
  List dropped_;
};
}
}

#endif
