#ifndef KEYTOKEY_HPP
#define KEYTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class KeyToKey final : public RemapFuncBase {
public:
  static void static_initialize(IOWorkLoop& workloop);
  static void static_terminate(void);

  KeyToKey(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_KEYTOKEY, autogenId),
                                  index_(0),
                                  currentToEvent_(CurrentToEvent::TO_KEYS),
                                  keyboardRepeatID_(-1),
                                  isRepeatEnabled_(true),
                                  lastPhysicalEventType_(PhysicalEventType::DOWN),
                                  delayUntilRepeat_(-1),
                                  keyRepeat_(-1) {}

  virtual ~KeyToKey(void) {
    if (target_ == this) {
      fire_timer_.cancelTimeout();
      target_ = nullptr;
    }
  }

  void prepare(RemapParams& remapParams) override;
  bool remap(RemapParams& remapParams) override;

  // ----------------------------------------
  // [0] => fromEvent_
  // [1] => toKeys_[0]
  // [2] => toKeys_[1]
  // [3] => ...
  void add(AddDataType datatype, AddValue newval) override;

  // ----------------------------------------
  // utility functions
  void add(KeyCode newval) { add(AddDataType(BRIDGE_DATATYPE_KEYCODE), AddValue(newval.get())); }
  void add(Option newval) { add(AddDataType(BRIDGE_DATATYPE_OPTION), AddValue(newval.get())); }

  bool call_remap_with_VK_PSEUDO_KEY(EventType eventType, PhysicalEventType physicalEventType);

  size_t toKeysSize(void) const { return toKeys_.size(); }
  void clearToKeys(void);
  bool isLastToEventModifierKeyOrLikeModifier(void) const {
    if (toKeys_.empty()) return false;

    auto& lastToEvent = toKeys_[toKeys_.size() - 1];
    if (lastToEvent.getModifierFlag() != ModifierFlag::ZERO) {
      return true;
    }
    if (lastToEvent.isEventLikeModifier()) {
      return true;
    }

    return false;
  }

  bool isPressing(void) const { return fromEvent_.isPressing(); }

private:
  static void fire_timer_callback(OSObject* owner, IOTimerEventSource* sender);
  void doDelayedAction(const Vector_ToEvent& keys, bool delayedActionCanceledBy);

  int getDelayUntilRepeat(void);
  int getKeyRepeat(void);

  class CurrentToEvent final {
  public:
    enum Value {
      TO_KEYS,
      BEFORE_KEYS,
      AFTER_KEYS,
      DELAYED_ACTION_KEYS,
      DELAYED_ACTION_CANCELED_DEFAULT_KEYS,
      DELAYED_ACTION_CANCELED_BY_KEYS,
      INCREASE_MODIFIER_FLAGS,
    };
  };

  Vector_ToEvent& getCurrentToEvent(void) {
    switch (currentToEvent_) {
    case CurrentToEvent::TO_KEYS:
      return toKeys_;
    case CurrentToEvent::BEFORE_KEYS:
      return beforeKeys_;
    case CurrentToEvent::AFTER_KEYS:
      return afterKeys_.back();
    case CurrentToEvent::DELAYED_ACTION_KEYS:
      return delayedActionKeys_;
    case CurrentToEvent::DELAYED_ACTION_CANCELED_DEFAULT_KEYS:
      return delayedActionCanceledDefaultKeys_;
    case CurrentToEvent::DELAYED_ACTION_CANCELED_BY_KEYS:
      return delayedActionCanceledByKeys_.back();
    case CurrentToEvent::INCREASE_MODIFIER_FLAGS:
      return increaseModifierFlags_;
    }
  }

  size_t index_;

  FromEvent fromEvent_;
  Vector_ModifierFlag fromModifierFlags_;
  Vector_ModifierFlag pureFromModifierFlags_; // fromModifierFlags_ - fromEvent_.getModifierFlag()

  Vector_ToEvent toKeys_;
  Vector_ToEvent beforeKeys_;
  Vector_Vector_ToEvent afterKeys_;
  Vector_ToEvent delayedActionKeys_;
  Vector_ToEvent delayedActionCanceledDefaultKeys_;
  Vector_Vector_ToEvent delayedActionCanceledByKeys_;
  Vector_ToEvent increaseModifierFlags_;
  CurrentToEvent::Value currentToEvent_;

  static TimerWrapper fire_timer_;
  static KeyToKey* target_;
  static FlagStatus flagStatusForDelayedActionKeys_;

  int keyboardRepeatID_;
  bool isRepeatEnabled_;
  PhysicalEventType lastPhysicalEventType_;

  int delayUntilRepeat_;
  int keyRepeat_;
};
}
}

#endif
