#ifndef EVENTOUTPUTQUEUE_HPP
#define EVENTOUTPUTQUEUE_HPP

#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "FlagStatus.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "List.hpp"
#include "ParamsUnion.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class EventOutputQueue {
    friend class PressDownKeys;

  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    // ======================================================================
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
      enum {
        // see IOHIPointing.cpp in darwin.
        POINTING_FIXED_SCALE = 65536, // (== << 16)
        POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)

        DELTA_SCALE = 128,
      };
      static void fire(const Params_ScrollWheelEventCallback& params);
      static void fire(int delta1, int delta2);
    };

    // ======================================================================
  private:
    enum {
      DELAY = 1,
    };
    class Item : public List::Item {
    public:
      Item(const Params_KeyboardEventCallBack& p)        : params(p) {}
      Item(const Params_UpdateEventFlagsCallback& p)     : params(p) {}
      Item(const Params_KeyboardSpecialEventCallback& p) : params(p) {}
      Item(const Params_RelativePointerEventCallback& p) : params(p) {}
      Item(const Params_ScrollWheelEventCallback& p)     : params(p) {}

      virtual ~Item(void) {}

      ParamsUnion params;
    };

    static void fire(OSObject* owner, IOTimerEventSource* sender);
    static void push(const Params_KeyboardEventCallBack& p);
    static void push(const Params_UpdateEventFlagsCallback& p);
    static void push(const Params_KeyboardSpecialEventCallback& p);
    static void push(const Params_RelativePointerEventCallback& p);
    static void push(const Params_ScrollWheelEventCallback& p);

    static List* queue_;
    static TimerWrapper timer_;
  };
}

#endif
