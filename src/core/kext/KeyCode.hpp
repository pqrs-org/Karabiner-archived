#ifndef KEYCODE_HPP
#define KEYCODE_HPP

#include "../../../src/bridge/include/bridge.h"
#include "Types.hpp"
#include "Vector.hpp"

namespace org_pqrs_Karabiner {
class KeyCode;
class Flags;
class Buttons;

// ======================================================================
class EventType final {
public:
  explicit EventType(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(EventType other) const { return value_ == other.get(); }
  bool operator!=(EventType other) const { return !(*this == other); }

  bool isKeyDownOrModifierDown(KeyCode key, Flags flags) const;

#include "../../../src/bridge/output/include.kext.EventType.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class KeyboardType final {
public:
  explicit KeyboardType(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(KeyboardType other) const { return value_ == other.get(); }
  bool operator!=(KeyboardType other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.KeyboardType.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class ModifierFlag final {
public:
  explicit ModifierFlag(unsigned int v = 0) : value_(v) {}
  explicit ModifierFlag(AddDataType datatype, AddValue newval) {
    if (datatype == BRIDGE_DATATYPE_MODIFIERFLAG) {
      value_ = newval;
    } else {
      value_ = 0;
    }
  }
  bool operator==(ModifierFlag other) const { return value_ == other.get(); }
  bool operator!=(ModifierFlag other) const { return !(*this == other); }

  KeyCode getKeyCode(void) const;
  unsigned int getRawBits(void) const;

#include "../../../src/bridge/output/include.kext.ModifierFlag.hpp"

private:
  unsigned int get(void) const { return value_; }
  unsigned int value_;
};
DECLARE_VECTOR_WITH_HELPER(ModifierFlag);
DECLARE_VECTOR(Vector_ModifierFlag);

class Flags final {
public:
  explicit Flags(unsigned int v = 0) : value_(v) {}
  explicit Flags(ModifierFlag v) : value_(v.getRawBits()) {}
  explicit Flags(const Vector_ModifierFlag& v) {
    value_ = 0;
    for (size_t i = 0; i < v.size(); ++i) {
      value_ |= v[i].getRawBits();
    }
  }
  unsigned int get(void) const { return value_; }
  bool operator==(Flags other) const { return value_ == other.get(); }
  bool operator!=(Flags other) const { return !(*this == other); }

  unsigned int operator~(void) const { return ~value_; }
  Flags operator|(Flags other) const { return Flags(value_ | other.get()); }
  Flags operator|(ModifierFlag other) const { return *this | Flags(other); }
  Flags operator&(Flags other) const { return Flags(value_ & other.get()); }
  Flags operator&(ModifierFlag other) const { return *this & Flags(other); }

  Flags& add(Flags flags) {
    value_ |= flags.get();
    return *this;
  }
  Flags& add(ModifierFlag flag) { return add(Flags(flag)); }
  Flags& add(AddDataType datatype, AddValue newval) {
    if (datatype == AddDataType(BRIDGE_DATATYPE_MODIFIERFLAG)) {
      add(ModifierFlag(datatype, newval));
    }
    return *this;
  }
  Flags& remove(ModifierFlag flag);

  Flags& stripFN(void) { return remove(ModifierFlag::FN); }
  Flags& stripNUMPAD(void) { return remove(ModifierFlag::NUMPAD); }

  bool isOn(ModifierFlag flag) const {
    if (flag == ModifierFlag::ZERO) return true;

    unsigned int bits = flag.getRawBits();
    if (bits == 0) return false;

    return (value_ & bits) == bits;
  }
  bool isOn(Flags flags) const {
    return (*this & flags) == flags;
  }

private:
  unsigned int value_;
};

// ======================================================================
class KeyCode final {
public:
  explicit KeyCode(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(KeyCode other) const { return value_ == other.get(); }
  bool operator!=(KeyCode other) const { return !(*this == other); }

  bool operator>(KeyCode other) const { return value_ > other.get(); }
  bool operator>=(KeyCode other) const { return value_ >= other.get(); }

  static void normalizeKey(KeyCode& key, Flags& flags, EventType eventType, KeyboardType keyboardType);
  static void reverseNormalizeKey(KeyCode& key, Flags& flags, EventType eventType, KeyboardType keyboardType);

  ModifierFlag getModifierFlag(void) const;
  bool isModifier(void) const { return getModifierFlag() != ModifierFlag::ZERO; }

#include "../../../src/bridge/output/include.kext.KeyCode.hpp"

  // When FN key and Arrow key were pushed together, another key code was sent (Home,End,PageUp,PageDown or something).
  // We need to change these Home,End,PageUp,PageDown keys to FN+Arrow key before sending key code to remapper.
  // (If we change FN to Control_L, FN+Up-Arrow must be changed to Control_L+Up-Arrow. Not Control_L+PageUp).
  // We also need to change FN+Arrow to Home,End,PageUp,PageDown before outputting key code.
  //
  // This class handles the above.
  class FNKeyHack final {
  public:
    FNKeyHack(const KeyCode& fk, const KeyCode& tk) : fromKeyCode_(fk), toKeyCode_(tk), active_normalize_(false), active_reverse_(false) {}
    // FN+PageUp to FN+Up-Arrow
    bool normalize(KeyCode& key, Flags flags, EventType eventType) { return remap(key, flags, eventType, active_normalize_, fromKeyCode_, toKeyCode_); }
    // FN+Up-Arrow to PageUp
    bool reverse(KeyCode& key, Flags flags, EventType eventType) { return remap(key, flags, eventType, active_reverse_, toKeyCode_, fromKeyCode_); }

  private:
    bool remap(KeyCode& key, Flags flags, EventType eventType, bool& active, KeyCode fromKeyCode, KeyCode toKeyCode);

    const KeyCode& fromKeyCode_;
    const KeyCode& toKeyCode_;
    bool active_normalize_;
    bool active_reverse_;
  };

private:
  unsigned int value_;
};

class CharCode final {
public:
  explicit CharCode(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(CharCode other) const { return value_ == other.get(); }
  bool operator!=(CharCode other) const { return !(*this == other); }

private:
  unsigned int value_;
};
class CharSet final {
public:
  explicit CharSet(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(CharSet other) const { return value_ == other.get(); }
  bool operator!=(CharSet other) const { return !(*this == other); }

private:
  unsigned int value_;
};
class OrigCharCode final {
public:
  explicit OrigCharCode(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(OrigCharCode other) const { return value_ == other.get(); }
  bool operator!=(OrigCharCode other) const { return !(*this == other); }

private:
  unsigned int value_;
};
class OrigCharSet final {
public:
  explicit OrigCharSet(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(OrigCharSet other) const { return value_ == other.get(); }
  bool operator!=(OrigCharSet other) const { return !(*this == other); }

private:
  unsigned int value_;
};

// ======================================================================
class ConsumerKeyCode final {
public:
  explicit ConsumerKeyCode(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(ConsumerKeyCode other) const { return value_ == other.get(); }
  bool operator!=(ConsumerKeyCode other) const { return !(*this == other); }

  bool operator>(ConsumerKeyCode other) const { return value_ > other.get(); }
  bool operator>=(ConsumerKeyCode other) const { return value_ >= other.get(); }

  bool isRepeatable(void) const;

#include "../../../src/bridge/output/include.kext.ConsumerKeyCode.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class PointingButton final {
public:
  explicit PointingButton(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(PointingButton other) const { return value_ == other.get(); }
  bool operator!=(PointingButton other) const { return !(*this == other); }

  unsigned int operator~(void) const { return ~value_; }

#include "../../../src/bridge/output/include.kext.PointingButton.hpp"

private:
  unsigned int value_;
};
class Buttons final {
public:
  explicit Buttons(unsigned int v = 0) : value_(v) {}
  explicit Buttons(PointingButton v) : value_(v.get()) {}
  unsigned int get(void) const { return value_; }
  bool operator==(Buttons other) const { return value_ == other.get(); }
  bool operator!=(Buttons other) const { return !(*this == other); }

  unsigned int operator~(void) const { return ~value_; }
  Buttons operator|(Buttons other) const { return Buttons(value_ | other.get()); }
  Buttons operator|(PointingButton other) const { return *this | Buttons(other); }

  Buttons& add(Buttons buttons) {
    value_ |= buttons.get();
    return *this;
  }
  Buttons& add(PointingButton button) { return add(Buttons(button)); }
  Buttons& remove(Buttons buttons) {
    value_ &= ~(buttons.value_);
    return *this;
  }
  Buttons& remove(PointingButton button) { return remove(Buttons(button)); }

  bool isNONE(void) const { return value_ == 0; }
  bool isOn(Buttons buttons) const {
    return (value_ & buttons.get()) == buttons.get();
  }
  bool isOn(PointingButton button) const { return isOn(Buttons(button)); }

  Buttons justPressed(Buttons previous) const {
    return Buttons(value_ & ~(previous.get()));
  }
  Buttons justReleased(Buttons previous) const {
    return Buttons(~value_ & (previous.get()));
  }

  // population count
  unsigned int count(void) const {
    unsigned int bits = value_;

    bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
    bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
    bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
    bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
    return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);
  }

private:
  unsigned int value_;
};
inline Buttons operator|(PointingButton lhs, PointingButton rhs) { return Buttons(lhs.get() | rhs.get()); }

// ======================================================================
class ScrollWheel final {
public:
  explicit ScrollWheel(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(ScrollWheel other) const { return value_ == other.get(); }
  bool operator!=(ScrollWheel other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.ScrollWheel.hpp"

  static ScrollWheel getScrollWheelFromDelta(int fixedDelta1, int fixedDelta2) {
    // Example of delta:
    //   UP:    fixedDelta(106285,0,0)
    //   DOWN:  fixedDelta(-87489,0,0)
    //   LEFT:  fixedDelta(0,33195,0)
    //   RIGHT: fixedDelta(0,-49241,0)
    //
    //   DIAGONAL_NE: fixedDelta(43238,-50580,0)

    if (fixedDelta1 == 0 && fixedDelta2 == 0) return ScrollWheel::NONE;

    int abs_vertical = fixedDelta1 > 0 ? fixedDelta1 : -fixedDelta1;
    int abs_horizontal = fixedDelta2 > 0 ? fixedDelta2 : -fixedDelta2;

    if (abs_horizontal > abs_vertical) {
      // LEFT or RIGHT
      if (fixedDelta2 > 0) {
        return ScrollWheel::LEFT;
      } else {
        return ScrollWheel::RIGHT;
      }
    } else {
      // UP or DOWN
      if (fixedDelta1 > 0) {
        return ScrollWheel::UP;
      } else {
        return ScrollWheel::DOWN;
      }
    }

    // Never reach.
    return ScrollWheel::NONE;
  }

private:
  unsigned int value_;
};

// ======================================================================
class PointingRelative final {
public:
  explicit PointingRelative(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(PointingRelative other) const { return value_ == other.get(); }
  bool operator!=(PointingRelative other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.PointingRelative.hpp"

  static PointingRelative getPointingRelativeFromDelta(int dx, int dy) {
    // Example of delta:
    //   UP:    (dx:   4, dy: -25)
    //   DOWN:  (dx:   5, dy:  78)
    //   LEFT:  (dx: -36, dy:  -6)
    //   RIGHT: (dx:  80, dy:  16)

    if (dx == 0 && dy == 0) return PointingRelative::NONE;

    int abs_dx = dx > 0 ? dx : -dx;
    int abs_dy = dy > 0 ? dy : -dy;

    // Ignore diagonal direction.
    if (abs_dx > abs_dy) {
      if (abs_dy * 3 > abs_dx) return PointingRelative::NONE;
    } else {
      if (abs_dx * 3 > abs_dy) return PointingRelative::NONE;
    }

    if (abs_dx > abs_dy) {
      // LEFT or RIGHT
      if (dx < 0) {
        return PointingRelative::LEFT;
      } else {
        return PointingRelative::RIGHT;
      }
    } else {
      // UP or DOWN
      if (dy < 0) {
        return PointingRelative::UP;
      } else {
        return PointingRelative::DOWN;
      }
    }

    // Never reach.
    return PointingRelative::NONE;
  }

private:
  unsigned int value_;
};

// ======================================================================
class Option final {
public:
  explicit Option(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(Option other) const { return value_ == other.get(); }
  bool operator!=(Option other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.Option.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class ApplicationType final {
public:
  explicit ApplicationType(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(ApplicationType other) const { return value_ == other.get(); }
  bool operator!=(ApplicationType other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.ApplicationType.hpp"

private:
  unsigned int value_;
};

class WindowName final {
public:
  explicit WindowName(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(WindowName other) const { return value_ == other.get(); }
  bool operator!=(WindowName other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.WindowName.hpp"

private:
  unsigned int value_;
};

class UIElementRole final {
public:
  explicit UIElementRole(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(UIElementRole other) const { return value_ == other.get(); }
  bool operator!=(UIElementRole other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.UIElementRole.hpp"

private:
  unsigned int value_;
};

class ShellCommand final {
public:
  explicit ShellCommand(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(ShellCommand other) const { return value_ == other.get(); }
  bool operator!=(ShellCommand other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.ShellCommand.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class InputSource final {
public:
  explicit InputSource(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(InputSource other) const { return value_ == other.get(); }
  bool operator!=(InputSource other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.InputSource.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class DeviceVendor final {
public:
  explicit DeviceVendor(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(DeviceVendor other) const { return value_ == other.get(); }
  bool operator!=(DeviceVendor other) const { return !(*this == other); }

  bool isApple(void) const;

#include "../../../src/bridge/output/include.kext.DeviceVendor.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class DeviceProduct final {
public:
  explicit DeviceProduct(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(DeviceProduct other) const { return value_ == other.get(); }
  bool operator!=(DeviceProduct other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.DeviceProduct.hpp"

private:
  unsigned int value_;
};

// ======================================================================
// DeviceLocation is an identifier used to distinguish between
// multiple devices which have same DeviceVendor and DeviceProduct.
//
// -----------------------------------------------------------------------------
// From IOUSBDevice.cpp:
// LocationID is used to uniquely identify a device or interface and it's
// suppose to remain constant across reboots as long as the USB topology doesn't
// change.  It is a 32-bit word.  The top 2 nibbles (bits 31:24) represent the
// USB Bus Number.  Each nibble after that (e.g. bits 23:20 or 19:16) correspond
// to the port number of the hub that the device is connected to.
// -----------------------------------------------------------------------------
//
// LocationID for Bluetooth devices:
//   Device Address: 00-25-bc-f9-8f-34
//   LocationID:     0x3cf98f34
//
//   Device Address: 00-26-bb-7e-95-74
//   LocationID:     0x3b7e9574
//
class DeviceLocation final {
public:
  explicit DeviceLocation(unsigned int v = 0) : value_(v) {}
  unsigned int get(void) const { return value_; }
  bool operator==(DeviceLocation other) const { return value_ == other.get(); }
  bool operator!=(DeviceLocation other) const { return !(*this == other); }

#include "../../../src/bridge/output/include.kext.DeviceLocation.hpp"

private:
  unsigned int value_;
};

// ======================================================================
class DeviceIdentifier final {
public:
  DeviceIdentifier(void) : vendor_(0), product_(0), location_(0) {}
  DeviceIdentifier(DeviceVendor v, DeviceProduct p, DeviceLocation l) : vendor_(v), product_(p), location_(l) {}

  DeviceVendor getVendor(void) const { return vendor_; }
  DeviceProduct getProduct(void) const { return product_; }
  DeviceLocation getLocation(void) const { return location_; }

  void setVendor(DeviceVendor v) { vendor_ = v; }
  void setProduct(DeviceProduct v) { product_ = v; }
  void setLocation(DeviceLocation v) { location_ = v; }

  bool isEqualVendorProduct(DeviceVendor v, DeviceProduct p) const { return vendor_ == v && product_ == p; }
  bool isEqualVendor(DeviceVendor v) const { return vendor_ == v; }
  bool isEqualLocation(DeviceLocation l) const { return location_ == l; }
  bool isEqual(DeviceVendor v, DeviceProduct p = DeviceProduct::ANY, DeviceLocation l = DeviceLocation::ANY) const {
    if (vendor_ != v) return false;
    if (DeviceProduct::ANY != p && product_ != p) return false;
    if (DeviceLocation::ANY != l && location_ != l) return false;
    return true;
  }
  bool isEqual(const DeviceIdentifier& v) const {
    return isEqual(v.vendor_, v.product_, v.location_);
  }
  bool isApple(void) const { return vendor_.isApple(); }

private:
  DeviceVendor vendor_;
  DeviceProduct product_;
  DeviceLocation location_;
};
// For <device_only>, <device_not>
DECLARE_VECTOR(DeviceIdentifier);

// ======================================================================
namespace DeviceType {
enum DeviceType {
  UNKNOWN,
  APPLE_MIKEY_HID_DRIVER,
  APPLE_INTERNAL,
  APPLE_EXTERNAL,
  USB_OVERDRIVE,
};
}
}

#endif
