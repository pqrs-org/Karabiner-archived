#ifndef SCROLLWHEELTOKEY_HPP
#define SCROLLWHEELTOKEY_HPP

#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"
#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class ScrollWheelToKey : public RemapFuncBase {
    public:
      ScrollWheelToKey(void) :
        RemapFuncBase(BRIDGE_REMAPTYPE_SCROLLWHEELTOKEY),
        index_(0),
        firstScrollWheelEvent_(ScrollWheel::NONE),
        isContinuousScrollEventRemapped_(false)
      {
        continuousScrollEvent_ic_.begin();
        keyrepeat_ic_.begin();
        keytokey_.add(KeyCode::VK_PSEUDO_KEY);
      }

      bool remap(RemapParams& remapParams);

      void add(AddDataType datatype, AddValue newval);

    private:
      enum {
        CONTINUOUS_SCROLLWHEEL_EVENT_THRESHOLD = 20,
      };

      size_t index_;
      Vector_ModifierFlag fromModifierFlags_;
      ScrollWheel fromScrollWheel_;

      IntervalChecker continuousScrollEvent_ic_;
      ScrollWheel firstScrollWheelEvent_;
      bool isContinuousScrollEventRemapped_;

      IntervalChecker keyrepeat_ic_;

      KeyToKey keytokey_;
    };
  }
}

#endif
