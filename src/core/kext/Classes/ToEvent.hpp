#ifndef TOEVENT_HPP
#define TOEVENT_HPP

#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "Vector.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
class ToEvent final {
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

  ToEvent(void) : type_(Type::NONE) {}
  explicit ToEvent(KeyCode v) : type_(Type::KEY), key_(v) {}
  explicit ToEvent(ConsumerKeyCode v) : type_(Type::CONSUMER_KEY), consumer_(v) {}
  explicit ToEvent(PointingButton v) : type_(Type::POINTING_BUTTON), button_(v) {}

  ToEvent(unsigned int datatype, unsigned int v) {
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
      IOLOG_ERROR("Unknown datatype: %d\n", datatype);
      type_ = Type::NONE;
      break;
    }
  }

  const Vector_ModifierFlag& getModifierFlags(void) const { return modifierFlags_; }
  void addModifierFlag(AddDataType datatype, AddValue newval) {
    modifierFlags_.push_back(ModifierFlag(datatype, newval));
  }

  Type::Value getType(void) const { return type_; }

  // Get ModifierFlag from KeyCode.
  ModifierFlag getModifierFlag(void) const {
    if (type_ != Type::KEY) return ModifierFlag::ZERO;
    return key_.getModifierFlag();
  }
  bool isEventLikeModifier(void) const;

  void fire(EventType eventType, Flags flags, AutogenId autogenId, PhysicalEventType physicalEventType,
            bool add_to_keyrepeat, int delayUntilRepeat = 0, int keyRepeat = 0);
  void fire_downup(AutogenId autogenId, PhysicalEventType physicalEventType, bool add_to_keyrepeat = false) const;

  bool operator==(const Params_Base& paramsBase) const {
    switch (type_) {
    case Type::KEY: {
      auto p = paramsBase.get_Params_KeyboardEventCallBack();
      if (p) {
        return key_ == p->key;
      }
      break;
    }

    case Type::CONSUMER_KEY: {
      auto p = paramsBase.get_Params_KeyboardSpecialEventCallback();
      if (p) {
        return consumer_ == p->key;
      }
      break;
    }

    case Type::POINTING_BUTTON: {
      auto p = paramsBase.get_Params_RelativePointerEventCallback();
      if (p) {
        return button_ == p->ex_button;
      }
      break;
    }

    case Type::NONE:
      break;
    }
    return false;
  }

  bool operator==(KeyCode other) const {
    return type_ == Type::KEY && key_ == other;
  }

private:
  Type::Value type_;
  Vector_ModifierFlag modifierFlags_;
  KeyCode key_;
  ConsumerKeyCode consumer_;
  PointingButton button_;
};

DECLARE_VECTOR(ToEvent);
DECLARE_VECTOR(Vector_ToEvent);
}

#endif
