#ifndef FROMEVENT_HPP
#define FROMEVENT_HPP

#include "FlagStatus.hpp"
#include "IOLogWrapper.hpp"
#include "List.hpp"
#include "Params.hpp"
#include "Vector.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
class FromEvent;

class PressingFromEvents final {
  friend class FromEvent;

public:
  class Item final : public List::Item {
  public:
    Item(const FromEvent* p) : fromEvent_(p) {}
    ~Item(void) {}

    const FromEvent* getFromEvent(void) const { return fromEvent_; }

  private:
    const FromEvent* fromEvent_;
  };

  static Item* find(const FromEvent* p) {
    for (Item* q = static_cast<Item*>(list_.safe_front()); q; q = static_cast<Item*>(q->getnext())) {
      if (q->getFromEvent() == p) {
        return q;
      }
    }
    return nullptr;
  }

  static void clear(void) {
    list_.clear();
  }

  static void push_back(const FromEvent* p) {
    list_.push_back(new Item(p));
  }

  static bool erase(const FromEvent* p) {
    Item* q = find(p);
    if (q) {
      list_.erase_and_delete(q);
      return true;
    }
    return false;
  }

  static void erase_all(const FromEvent* p) {
    for (;;) {
      if (!erase(p)) {
        break;
      }
    }
  }

private:
  static List list_;
};

class FromEvent final {
public:
  class Type final {
  public:
    enum Value {
      NONE,
      KEY,
      CONSUMER_KEY, // Mute, VolumeIncrement, VolumeDecrement, etcetc.
      POINTING_BUTTON,
    };
  };

  FromEvent(void) : type_(Type::NONE) {}
  explicit FromEvent(KeyCode v) : type_(Type::KEY), key_(v) {}
  explicit FromEvent(ConsumerKeyCode v) : type_(Type::CONSUMER_KEY), consumer_(v) {}
  explicit FromEvent(PointingButton v) : type_(Type::POINTING_BUTTON), button_(v) {}

  explicit FromEvent(const Params_Base& paramsBase) {
    type_ = Type::NONE;

    {
      auto p = paramsBase.get_Params_KeyboardEventCallBack();
      if (p) {
        type_ = Type::KEY;
        key_ = p->key;
        return;
      }
    }
    {
      auto p = paramsBase.get_Params_KeyboardSpecialEventCallback();
      if (p) {
        type_ = Type::CONSUMER_KEY;
        consumer_ = p->key;
        return;
      }
    }
    {
      auto p = paramsBase.get_Params_RelativePointerEventCallback();
      if (p) {
        type_ = Type::POINTING_BUTTON;
        button_ = p->ex_button;
        return;
      }
    }
  }

  FromEvent(AddDataType datatype, AddValue v) {
    switch (datatype) {
    case BRIDGE_DATATYPE_KEYCODE:
      type_ = Type::KEY;
      key_ = KeyCode(v);
      break;
    case BRIDGE_DATATYPE_CONSUMERKEYCODE:
      type_ = Type::CONSUMER_KEY;
      consumer_ = ConsumerKeyCode(v);
      break;
    case BRIDGE_DATATYPE_POINTINGBUTTON:
      type_ = Type::POINTING_BUTTON;
      button_ = PointingButton(v);
      break;
    default:
      IOLOG_ERROR("Unknown datatype: %u\n", static_cast<unsigned int>(datatype));
      type_ = Type::NONE;
      break;
    }
  }

  ~FromEvent(void) { PressingFromEvents::erase_all(this); }

  Type::Value getType(void) const { return type_; }

  // Return whether pressing state is changed.
  bool changePressingState(const Params_Base& paramsBase,
                           const FlagStatus& currentFlags,
                           const Vector_ModifierFlag& fromFlags);

  // Caution:
  // `isPressing` may return true at key up event if you connected multiple keyboards and pressing the same key.
  //
  // For example,
  //  1. Press the return key in keyboard1.   (isPressing == true)
  //  2. Press the return key in keyboard2.   (isPressing == true)
  //  3. Release the return key in keyboard1. (isPressing == true)
  //  4. Release the return key in keyboard2. (isPressing == false)
  //
  // So you must not use `isPressing` to detect the input event is key down or key up.
  // (You should use `Params_Base::iskeydown` to determine key down or key up.)
  bool isPressing(void) const { return PressingFromEvents::find(this); }

  // Primitive functions:
  // These functions do not treat Flags.
  // Use changePressingState in general.
  bool isTargetEvent(const Params_Base& paramsBase) const;
  bool isTargetDownEvent(const Params_Base& paramsBase) const;
  bool isTargetUpEvent(const Params_Base& paramsBase) const;

  // Get ModifierFlag from KeyCode.
  ModifierFlag getModifierFlag(void) const {
    if (type_ != Type::KEY) return ModifierFlag::ZERO;
    return key_.getModifierFlag();
  }
  PointingButton getPointingButton(void) const {
    if (type_ != Type::POINTING_BUTTON) return PointingButton::NONE;
    return button_;
  }

private:
  bool isTargetEvent(bool& isDown, const Params_Base& paramsBase) const;

  // Do not store Flags in FromEvent because SimultaneousKeyPresses uses multiple FromEvents.

  Type::Value type_;
  KeyCode key_;
  ConsumerKeyCode consumer_;
  PointingButton button_;
};

DECLARE_VECTOR(FromEvent);
}

#endif
