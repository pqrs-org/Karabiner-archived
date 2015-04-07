#ifndef DROPKEY_HPP
#define DROPKEY_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropKey final : public RemapFuncBase {
public:
  DropKey(void) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPKEY),
                  dropKey_(false),
                  dropConsumerKey_(false),
                  dropPointingButton_(false) {}

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  bool dropKey(RemapParams& remapParams);

  class Item final : public List::Item {
  public:
    Item(const Params_Base& paramsBase) : fromEvent(paramsBase) {}

    FromEvent fromEvent;
  };

  Vector_ModifierFlag fromModifierFlags_;
  List dropped_;

  bool dropKey_;
  bool dropConsumerKey_;
  bool dropPointingButton_;
};
}
}

#endif
