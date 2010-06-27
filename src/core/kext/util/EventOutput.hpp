#ifndef EVENTOUTPUT_HPP
#define EVENTOUTPUT_HPP

#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace EventOutput {
    class FireModifiers {
    public:
      static void fire(Flags toFlags = FlagStatus::makeFlags(), KeyboardType keyboardType = CommonData::getcurrent_keyboardType());

    private:
      static Flags lastFlags_;
    };

    class FireKey {
    public:
      static void fire(const Params_KeyboardEventCallBack& params);
      static void fire_downup(Flags flags, KeyCode key, KeyboardType keyboardType = CommonData::getcurrent_keyboardType());
    };

    class FireConsumer {
    public:
      static void fire(const Params_KeyboardSpecialEventCallback& params);
    };

    class FireRelativePointer {
    public:
      static void fire(Buttons toButtons = ButtonStatus::makeButtons(), int dx = 0, int dy = 0);

    private:
      static Buttons lastButtons_;
    };

    class FireScrollWheel {
    public:
      static void fire(const Params_ScrollWheelEventCallback& params);
    };
  }
}

#endif
