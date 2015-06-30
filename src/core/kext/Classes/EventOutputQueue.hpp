#ifndef EVENTOUTPUTQUEUE_HPP
#define EVENTOUTPUTQUEUE_HPP

#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "EventInputQueue.hpp"
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
  class FireModifiers final {
  public:
    static void fire(AutogenId autogenId,
                     PhysicalEventType physicalEventType,
                     Flags toFlags = FlagStatus::globalFlagStatus().makeFlags(),
                     KeyboardType keyboardType = CommonData::getcurrent_keyboardType());

    static Flags getLastFlags(void) { return lastFlags_; }
    static void setIgnorePhysicalUpEvent(bool newval) { isIgnorePhysicalUpEvent_ = newval; }

  private:
    static Flags lastFlags_;
    static bool isIgnorePhysicalUpEvent_;
  };

  class FireKey final {
  public:
    static void fire(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  };

  class FireConsumer final {
  public:
    static void fire(const Params_KeyboardSpecialEventCallback& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  };

  class FireRelativePointer final {
  public:
    static void fire(AutogenId autogenId, PhysicalEventType physicalEventType, Buttons toButtons = ButtonStatus::makeButtons(), int dx = 0, int dy = 0);

  private:
    static Buttons lastButtons_;
  };

  class FireScrollWheel final {
  public:
    enum {
      // see IOHIPointing.cpp in darwin.
      POINTING_FIXED_SCALE = 65536, // (== << 16)
      POINTING_POINT_SCALE = 10,    // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
      DELTA_SCALE = 128,
    };
    static void fire(const Params_ScrollWheelEventCallback& params, AutogenId autogenId, PhysicalEventType physicalEventType);
    static void fire(int delta1, int delta2, AutogenId autogenId, PhysicalEventType physicalEventType);
  };

  class FireWait final {
  public:
    static void fire(const Params_Wait& params, AutogenId autogenId);
  };

  class Item final : public List::Item {
  public:
    Item(const Params_Base& p, AutogenId autogenId) : p_(Params_Factory::copy(p)),
                                                      autogenId_(autogenId),
                                                      eventInputQueueSerialNumber_(EventInputQueue::currentSerialNumber()),
                                                      canceled_(false) {}

    virtual ~Item(void) {
      if (p_) {
        delete p_;
      }
    }

    const Params_Base& getParamsBase(void) const { return Params_Base::safe_dereference(p_); }
    AutogenId getAutogenId(void) const { return autogenId_; }
    EventInputQueue::SerialNumber getEventInputQueueSerialNumber(void) const { return eventInputQueueSerialNumber_; }
    bool isCanceled(void) const { return canceled_; }
    void cancel(void) { canceled_ = true; }

  private:
    Item(const Item& rhs);            // Prevent copy-construction
    Item& operator=(const Item& rhs); // Prevent assignment

    const Params_Base* p_;
    const AutogenId autogenId_;
    const EventInputQueue::SerialNumber eventInputQueueSerialNumber_;
    bool canceled_;
  };

  static const List& getQueue(void) { return queue_; }

private:
  static void fire_timer_callback(OSObject* /* owner */, IOTimerEventSource* /* sender */);
  static unsigned int calcDelay(const Params_Base& params);
  static void push(const Params_KeyboardEventCallBack& p, AutogenId autogenId);
  static void push(const Params_KeyboardSpecialEventCallback& p, AutogenId autogenId);
  static void push(const Params_RelativePointerEventCallback& p, AutogenId autogenId);
  static void push(const Params_ScrollWheelEventCallback& p, AutogenId autogenId);
  static void push(const Params_Wait& p, AutogenId autogenId);

  static List queue_;
  static TimerWrapper fire_timer_;
  static Buttons previousButtons_;
};
}

#endif
