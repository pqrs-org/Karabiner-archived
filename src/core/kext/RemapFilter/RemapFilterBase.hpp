#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "bridge.h"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "Vector.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class RemapFilterBase {
protected:
  RemapFilterBase(unsigned int type) : type_(type) {}

public:
  virtual ~RemapFilterBase(void) {}

  virtual bool isblocked(void) = 0;
  virtual bool isblocked_keyup(void) {
    // Usually, when keyup, we should not block by filter.
    //
    // For example:
    // <item>
    //   <modifier_only>ModifierFlag::SHIFT_L</modifier_only>
    //   <autogen>__KeyToKey__ KeyCode::RETURN, KeyCode::TAB</autogen>
    // </item>
    //
    // (1) SHIFT_L down
    // (2) RETURN down  -> change to tab
    // (3) SHIFT_L up
    // (4) RETURN key   -> should change even if shift is not pressed.
    //
    //
    // But, if multiple keyboards are connected,
    // we should apply <device_only>,<device_not> filter when keyup.
    //
    // For example:
    // <item>
    //   <device_only> Keyboard1 </device_only>
    //   <autogen>__KeyToKey__ KeyCode::RETURN, KeyCode::TAB</autogen>
    // </item>
    //
    // (1) RETURN from Keyboard1 -> change to tab
    // (2) RETURN from Keyboard2
    // (3) RETURN from Keyboard2 -> should not change.
    // (4) RETURN from Keyboard1

    return false;
  }

  unsigned int get_type(void) const { return type_; }

private:
  unsigned int type_;
};

typedef RemapFilterBase* RemapFilterBasePointer;
DECLARE_VECTOR(RemapFilterBasePointer);
}
}

#endif
