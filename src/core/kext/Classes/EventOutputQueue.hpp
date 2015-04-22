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

  static uint64_t getLastPushedSerialNumber(void) { return serialNumber_; }

  // ======================================================================
  class FireModifiers final {
  public:
    static void fire(Flags toFlags = FlagStatus::globalFlagStatus().makeFlags(),
                     KeyboardType keyboardType = CommonData::getcurrent_keyboardType());

    static Flags getLastFlags(void) { return lastFlags_; }

  private:
    static Flags lastFlags_;
  };

  class FireKey final {
  public:
    static void fire(const Params_KeyboardEventCallBack& params, AutogenId autogenId);

    // TODO: DELETE ME. Do not use this method.
    static void fire(const Params_KeyboardEventCallBack& params) {
      fire(params, AutogenId(0));
    }
  };

  class FireConsumer final {
  public:
    static void fire(const Params_KeyboardSpecialEventCallback& params);
  };

  class FireRelativePointer final {
  public:
    static void fire(Buttons toButtons = ButtonStatus::makeButtons(), int dx = 0, int dy = 0);

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
    static void fire(const Params_ScrollWheelEventCallback& params);
    static void fire(int delta1, int delta2);
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
    uint64_t getEventInputQueueSerialNumber(void) const { return eventInputQueueSerialNumber_; }
    bool isCanceled(void) const { return canceled_; }
    void cancel(void) { canceled_ = true; }

  private:
    Item(const Item& rhs);            // Prevent copy-construction
    Item& operator=(const Item& rhs); // Prevent assignment

    const Params_Base* p_;
    const AutogenId autogenId_;
    const uint64_t eventInputQueueSerialNumber_;
    bool canceled_;
  };

  static const List& getQueue(void) { return queue_; }

private:
  // Collapse continuous up,down modifier key events in the same EventInputQueue.
  //
  // For example, if EventInputQueue is follows,
  //   EventOutputQueue[0]: escape up
  //   EventOutputQueue[1]: shift up
  //   EventOutputQueue[2]: shift down
  //   EventOutputQueue[3]: tab down
  // queue will be collapsed.
  //   EventOutputQueue[0]: escape up
  //   EventOutputQueue[1]: shift up    (canceled)
  //   EventOutputQueue[2]: shift down  (canceled)
  //   EventOutputQueue[3]: tab down
  //
  // We need to do this in order to reduce unnecessary modifier events in the same time.
  //
  static void collapseModifierKeyUpDownEvent(void);

  static void fire_timer_callback(OSObject* /* owner */, IOTimerEventSource* /* sender */);
  static unsigned int calcDelay(const Params_Base& params);
  static void push(const Params_KeyboardEventCallBack& p);
  static void push(const Params_KeyboardSpecialEventCallback& p);
  static void push(const Params_RelativePointerEventCallback& p);
  static void push(const Params_ScrollWheelEventCallback& p);
  static void push(const Params_Wait& p, AutogenId autogenId);

  static List queue_;
  static TimerWrapper fire_timer_;
  static Buttons previousButtons_;

  // Increment at `push`.
  static uint64_t serialNumber_;
  static uint64_t lastProcessedEventInputQueueSerialNumber_;
};
}

#endif
