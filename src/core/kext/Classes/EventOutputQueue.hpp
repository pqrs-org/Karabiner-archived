#ifndef EVENTOUTPUTQUEUE_HPP
#define EVENTOUTPUTQUEUE_HPP

#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "FlagStatus.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "List.hpp"
#include "Params.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_Karabiner {
class EventOutputQueue final {
  friend class PressDownKeys;

public:
  static void initialize(IOWorkLoop& workloop);
  static void terminate(void);

  // ======================================================================
  class FireModifiers {
  public:
    static void fire(Flags toFlags = FlagStatus::globalFlagStatus().makeFlags(),
                     KeyboardType keyboardType = CommonData::getcurrent_keyboardType());

  private:
    static Flags lastFlags_;
  };

  class FireKey {
  public:
    static void fire(const Params_KeyboardEventCallBack& params);
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
    enum {
      // see IOHIPointing.cpp in darwin.
      POINTING_FIXED_SCALE = 65536, // (== << 16)
      POINTING_POINT_SCALE = 10,    // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
      DELTA_SCALE = 128,
    };
    static void fire(const Params_ScrollWheelEventCallback& params);
    static void fire(int delta1, int delta2);
  };

  class FireWait {
  public:
    static void fire(const Params_Wait& params);
  };

  // ======================================================================
private:
  class Item : public List::Item {
  public:
    Item(const Params_Base& p) : p_(Params_Factory::copy(p)) {}

    virtual ~Item(void) {
      if (p_) {
        delete p_;
      }
    }

    const Params_Base& getParamsBase(void) const { return Params_Base::safe_dereference(p_); }

  private:
    Item(const Item& rhs);            // Prevent copy-construction
    Item& operator=(const Item& rhs); // Prevent assignment

    const Params_Base* p_;
  };

  static void fire_timer_callback(OSObject* /* owner */, IOTimerEventSource* /* sender */);
  static unsigned int calcDelay(const Params_Base& params);
  static void push(const Params_KeyboardEventCallBack& p);
  static void push(const Params_KeyboardSpecialEventCallback& p);
  static void push(const Params_RelativePointerEventCallback& p);
  static void push(const Params_ScrollWheelEventCallback& p);
  static void push(const Params_Wait& p);

  static List queue_;
  static TimerWrapper fire_timer_;
  static Buttons previousButtons_;
};
}

#endif
