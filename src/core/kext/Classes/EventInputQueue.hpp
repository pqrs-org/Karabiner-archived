#ifndef EVENTINPUTQUEUE_HPP
#define EVENTINPUTQUEUE_HPP

#include "FromEvent.hpp"
#include "IntervalChecker.hpp"
#include "KeyCode.hpp"
#include "List.hpp"
#include "ListHookedDevice.hpp"
#include "Params.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class SimultaneousKeyPresses;
}

class EventInputQueue final {
  friend class RemapFunc::SimultaneousKeyPresses;

public:
  class SerialNumber final : public ::SerialNumber {};
  DECLARE_VECTOR_WITH_HELPER(SerialNumber);

  static void initialize(IOWorkLoop& workloop);
  static void terminate(void);

  static SerialNumber currentSerialNumber(void) { return serialNumber_; }

  class ScopedSerialNumberDecreaser {
  public:
    ScopedSerialNumberDecreaser(void) { --serialNumber_; }
    ~ScopedSerialNumberDecreaser(void) { ++serialNumber_; }
  };

  // ------------------------------------------------------------
  static void push_KeyboardEventCallback(OSObject* target,
                                         unsigned int eventType,
                                         unsigned int flags,
                                         unsigned int key,
                                         unsigned int charCode,
                                         unsigned int charSet,
                                         unsigned int origCharCode,
                                         unsigned int origCharSet,
                                         unsigned int keyboardType,
                                         bool repeat,
                                         AbsoluteTime ts,
                                         OSObject* sender,
                                         void* refcon);

  static void push_UpdateEventFlagsCallback(OSObject* target,
                                            unsigned flags,
                                            OSObject* sender,
                                            void* refcon);

  static void push_KeyboardSpecialEventCallback(OSObject* target,
                                                unsigned int eventType,
                                                unsigned int flags,
                                                unsigned int key,
                                                unsigned int flavor,
                                                UInt64 guid,
                                                bool repeat,
                                                AbsoluteTime ts,
                                                OSObject* sender,
                                                void* refcon);

  static void push_RelativePointerEventCallback(OSObject* target,
                                                int buttons,
                                                int dx,
                                                int dy,
                                                AbsoluteTime ts,
                                                OSObject* sender,
                                                void* refcon);

  static void push_ScrollWheelEventCallback(OSObject* target,
                                            short deltaAxis1,
                                            short deltaAxis2,
                                            short deltaAxis3,
                                            IOFixed fixedDelta1,
                                            IOFixed fixedDelta2,
                                            IOFixed fixedDelta3,
                                            SInt32 pointDelta1,
                                            SInt32 pointDelta2,
                                            SInt32 pointDelta3,
                                            SInt32 options,
                                            AbsoluteTime ts,
                                            OSObject* sender,
                                            void* refcon);

  // ------------------------------------------------------------
  class Item final : public List::Item {
  public:
    Item(const Params_Base& p, bool r, const DeviceIdentifier& di, const ListHookedDevice::WeakPointer_Item& device) : p_(Params_Factory::copy(p)),
                                                                                                                       retainFlagStatusTemporaryCount(r),
                                                                                                                       deviceIdentifier(di),
                                                                                                                       deviceWeakPointer(device),
                                                                                                                       isSimultaneousKeyPressesTarget(true),
                                                                                                                       enqueuedFrom(ENQUEUED_FROM_HARDWARE) {
      ic.begin();
    }

    Item(const Item& rhs) : p_(Params_Factory::copy(rhs.getParamsBase())),
                            retainFlagStatusTemporaryCount(rhs.retainFlagStatusTemporaryCount),
                            deviceIdentifier(rhs.deviceIdentifier),
                            deviceWeakPointer(rhs.deviceWeakPointer),
                            ic(rhs.ic),
                            isSimultaneousKeyPressesTarget(rhs.isSimultaneousKeyPressesTarget),
                            enqueuedFrom(rhs.enqueuedFrom) {}

    virtual ~Item(void) {
      if (p_) {
        delete p_;
      }
    }

    const Params_Base& getParamsBase(void) const { return Params_Base::safe_dereference(p_); }

    bool retainFlagStatusTemporaryCount;
    DeviceIdentifier deviceIdentifier;
    ListHookedDevice::WeakPointer_Item deviceWeakPointer;

    IntervalChecker ic;

    // Flag for __SimultaneousKeyPresses__
    bool isSimultaneousKeyPressesTarget;

    // To avoid recursive enqueueing from blockedQueue_.
    enum EnqueuedFrom {
      ENQUEUED_FROM_HARDWARE,
      ENQUEUED_FROM_BLOCKEDQUEUE,
    } enqueuedFrom;

  private:
    Item& operator=(const Item& rhs); // Prevent assignment

    const Params_Base* p_;
  };

private:
  enum DelayType {
    DELAY_TYPE_KEY,
    DELAY_TYPE_POINTING_BUTTON,
  };

  class BlockUntilKeyUpHandler final {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    // Return true if you need to call doFire.
    static bool doBlockUntilKeyUp(void);

    static bool isQueueEmpty(void) {
      return blockedQueue_.empty();
    }

  private:
    static bool isTargetDownEventInBlockedQueue(const Item& front);
    static bool isOrphanKeyUpEventExistsInBlockedQueue(void);
    static void endBlocking(void);
    static void setIgnoreToAllPressingEvents(void);
    static void blockingTimeOut_timer_callback(OSObject* owner, IOTimerEventSource* sender);

    class PressingEvent final : public List::Item {
    public:
      PressingEvent(const Params_Base& p) : p_(Params_Factory::copy(p)),
                                            ignore_(false) {
        fromEvent_ = FromEvent(Params_Base::safe_dereference(p_));
      }

      virtual ~PressingEvent(void) {
        if (p_) {
          delete p_;
        }
      }

      const Params_Base& getParamsBase(void) const { return Params_Base::safe_dereference(p_); }
      const FromEvent& getFromEvent(void) const { return fromEvent_; }

      bool ignore(void) const { return ignore_; }
      void setIgnore(void) { ignore_ = true; }

    private:
      PressingEvent(const PressingEvent& rhs);            // Prevent copy-construction
      PressingEvent& operator=(const PressingEvent& rhs); // Prevent assignment

      const Params_Base* p_;
      FromEvent fromEvent_;
      bool ignore_;
    };

    static List blockedQueue_;
    static List pressingEvents_;
    static TimerWrapper blockingTimeOut_timer_;
  };

  static uint32_t calcdelay(DelayType type);

  // ------------------------------------------------------------
  static void enqueue_(const Params_Base& paramsBase,
                       bool retainFlagStatusTemporaryCount, const DeviceIdentifier& di, const ListHookedDevice::WeakPointer_Item& device, bool push_back, bool isSimultaneousKeyPressesTarget);
  static void enqueue_(const Params_Base& paramsBase,
                       bool retainFlagStatusTemporaryCount, const DeviceIdentifier& di, const ListHookedDevice::WeakPointer_Item& device) {
    enqueue_(paramsBase, retainFlagStatusTemporaryCount, di, device, true, true);
  }
  static void fire_timer_callback(OSObject* owner, IOTimerEventSource* sender);
  static void doFire(void);
  static void resetInternalStateIfNeeded(void);
  static void setTimer(void);

  static List queue_;
  static IntervalChecker ic_;
  static TimerWrapper fire_timer_;
  // Increment at fire_timer_callback.
  static SerialNumber serialNumber_;
};
}

#endif
