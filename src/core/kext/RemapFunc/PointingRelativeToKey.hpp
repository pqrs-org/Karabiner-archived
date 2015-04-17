#ifndef POINTINGRELATIVETOKEY_HPP
#define POINTINGRELATIVETOKEY_HPP

#include "DeltaBuffer.hpp"
#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"
#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class PointingRelativeToKey final : public RemapFuncBase {
public:
  PointingRelativeToKey(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_POINTINGRELATIVETOKEY, autogenId),
                                               keytokey_{{autogenId},
                                                         {autogenId},
                                                         {autogenId},
                                                         {autogenId}},
                                               current_keytokey_(nullptr) {
    keyrepeat_ic_.begin();

    for (size_t i = 0; i < KEYTOKEY__END__; ++i) {
      keytokey_[i].add(KeyCode::VK_PSEUDO_KEY);
    }
  }

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  PointingRelativeToKey(const PointingRelativeToKey& rhs);            // Prevent copy-construction
  PointingRelativeToKey& operator=(const PointingRelativeToKey& rhs); // Prevent assignment

  Vector_ModifierFlag fromModifierFlags_;

  DeltaBuffer deltaBuffer_dx_;
  DeltaBuffer deltaBuffer_dy_;
  IntervalChecker keyrepeat_ic_;

  enum {
    KEYTOKEY_UP,
    KEYTOKEY_DOWN,
    KEYTOKEY_LEFT,
    KEYTOKEY_RIGHT,
    KEYTOKEY__END__,
  };
  KeyToKey keytokey_[KEYTOKEY__END__];
  KeyToKey* current_keytokey_;
};
}
}

#endif
