#ifndef SCROLLWHEELTOKEY_HPP
#define SCROLLWHEELTOKEY_HPP

#include "RemapFuncClasses.hpp"
#include "KeyToKey.hpp"
#include "IntervalChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class ScrollWheelToKey {
    public:
      ScrollWheelToKey(void);
      ~ScrollWheelToKey(void);

      bool remap(RemapPointingParams_scroll& remapParams);

      void add(unsigned int datatype, unsigned int newval);

    private:
      size_t index_;
      Flags fromFlags_;
      ScrollWheel fromScrollWheel_;
      IntervalChecker ic_;
      bool isLastEventRemapped_;

      KeyToKey keytokey_;
    };
  }
}

#endif
