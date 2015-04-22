#ifndef CALLBACKWRAPPER_HPP
#define CALLBACKWRAPPER_HPP

#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "Types.hpp"

namespace org_pqrs_Karabiner {
class Params_KeyboardEventCallBack;
class Params_UpdateEventFlagsCallback;
class Params_KeyboardSpecialEventCallback;
class Params_RelativePointerEventCallback;
class Params_ScrollWheelEventCallback;
class Params_Wait;

class Params_Base {
public:
  Params_Base(void) {}
  virtual ~Params_Base(void) {}

  virtual const Params_KeyboardEventCallBack* get_Params_KeyboardEventCallBack(void) const { return nullptr; }
  virtual const Params_UpdateEventFlagsCallback* get_Params_UpdateEventFlagsCallback(void) const { return nullptr; }
  virtual const Params_KeyboardSpecialEventCallback* get_Params_KeyboardSpecialEventCallback(void) const { return nullptr; }
  virtual const Params_RelativePointerEventCallback* get_Params_RelativePointerEventCallback(void) const { return nullptr; }
  virtual const Params_ScrollWheelEventCallback* get_Params_ScrollWheelEventCallback(void) const { return nullptr; }
  virtual const Params_Wait* get_Params_Wait(void) const { return nullptr; }

  virtual bool iskeydown(bool& output) const { return false; }
  virtual bool isModifier(void) const { return false; }
  virtual bool isRepeat(void) const { return false; }

  static const Params_Base& emptyInstance(void);
  static const Params_Base& safe_dereference(const Params_Base* p) { return p == nullptr ? emptyInstance() : *p; }
};

// =================================================
class Params_KeyboardEventCallBack final : public Params_Base {
  friend class EventOutputQueue;

public:
  Params_KeyboardEventCallBack(EventType et, Flags fl, KeyCode kc,
                               CharCode cc, CharSet cs, OrigCharCode occ, OrigCharSet ocs,
                               KeyboardType kt, bool r) : eventType(et), flags(fl), key(kc),
                                                          charCode(cc), charSet(cs), origCharCode(occ), origCharSet(ocs),
                                                          keyboardType(kt), repeat(r),
                                                          ex_iskeydown(et.isKeyDownOrModifierDown(kc, fl)) {}

  Params_KeyboardEventCallBack(EventType et, Flags fl, KeyCode kc,
                               KeyboardType kt, bool r) : eventType(et), flags(fl), key(kc),
                                                          charCode(0), charSet(0), origCharCode(0), origCharSet(0),
                                                          keyboardType(kt), repeat(r),
                                                          ex_iskeydown(et.isKeyDownOrModifierDown(kc, fl)) {}

  ~Params_KeyboardEventCallBack(void) {}

  const Params_KeyboardEventCallBack* get_Params_KeyboardEventCallBack(void) const override { return this; }
  bool iskeydown(bool& output) const override {
    output = ex_iskeydown;
    return true;
  }
  bool isModifier(void) const override { return key.isModifier(); }
  bool isRepeat(void) const override { return repeat; }

  // ----------------------------------------
  static void log(bool isCaught, EventType eventType, Flags flags, KeyCode key, KeyboardType keyboardType, bool repeat) {
    IOLOG_DEBUG("KeyboardEventCallback [%7s]: eventType %2d, flags 0x%08x, key 0x%04x, kbdType %3d, repeat = %d\n",
                isCaught ? "caught" : "sending",
                eventType.get(), flags.get(), key.get(), keyboardType.get(), repeat);
  }

  const EventType eventType;
  const Flags flags;
  const KeyCode key;
  const CharCode charCode;
  const CharSet charSet;
  const OrigCharCode origCharCode;
  const OrigCharSet origCharSet;
  const KeyboardType keyboardType;
  const bool repeat;

  const bool ex_iskeydown;
};

class Params_UpdateEventFlagsCallback final : public Params_Base {
  friend class EventOutputQueue;

public:
  Params_UpdateEventFlagsCallback(Flags fl) : flags(fl) {}

  ~Params_UpdateEventFlagsCallback(void) {}

  const Params_UpdateEventFlagsCallback* get_Params_UpdateEventFlagsCallback(void) const override { return this; }

  // ----------------------------------------
  static void log(bool isCaught, Flags flags) {
    IOLOG_DEBUG("UpdateEventFlagsCallback [%7s]: flags 0x%08x\n",
                isCaught ? "caught" : "sending",
                flags.get());
  }

  const Flags flags;
};

class Params_KeyboardSpecialEventCallback final : public Params_Base {
  friend class EventOutputQueue;

public:
  Params_KeyboardSpecialEventCallback(EventType et, Flags fl, ConsumerKeyCode ckc,
                                      unsigned int fv, UInt64 g,
                                      bool r) : eventType(et), flags(fl), key(ckc),
                                                flavor(fv), guid(g),
                                                repeat(r), ex_iskeydown(et == EventType::DOWN) {}

  Params_KeyboardSpecialEventCallback(EventType et, Flags fl, ConsumerKeyCode ckc,
                                      bool r) : eventType(et), flags(fl), key(ckc),
                                                flavor(ckc.get()), guid(static_cast<UInt64>(-1)),
                                                repeat(r), ex_iskeydown(et == EventType::DOWN) {}

  ~Params_KeyboardSpecialEventCallback(void) {}

  const Params_KeyboardSpecialEventCallback* get_Params_KeyboardSpecialEventCallback(void) const override { return this; }
  bool iskeydown(bool& output) const override {
    output = ex_iskeydown;
    return true;
  }
  bool isRepeat(void) const override { return repeat; }

  // ----------------------------------------
  static void log(bool isCaught, EventType eventType, Flags flags, ConsumerKeyCode key, unsigned int flavor, UInt64 guid, bool repeat) {
    IOLOG_DEBUG("KeyboardSpecialEventCallBack [%7s]: eventType %2d, flags 0x%08x, key 0x%04x, flavor %4d, guid %lld, repeat = %d\n",
                isCaught ? "caught" : "sending",
                eventType.get(), flags.get(), key.get(), flavor, guid, repeat);
  }

  const EventType eventType;
  const Flags flags;
  const ConsumerKeyCode key;
  const unsigned int flavor;
  const UInt64 guid;
  const bool repeat;

  const bool ex_iskeydown;
};

class Params_RelativePointerEventCallback final : public Params_Base {
  friend class EventOutputQueue;

public:
  Params_RelativePointerEventCallback(Buttons bt, int x, int y, PointingButton ex_btn, bool ex_isdown) : buttons(bt),
                                                                                                         dx(x), dy(y),
                                                                                                         ex_button(ex_btn), ex_isbuttondown(ex_isdown) {}

  ~Params_RelativePointerEventCallback(void) {}

  const Params_RelativePointerEventCallback* get_Params_RelativePointerEventCallback(void) const override { return this; }
  bool iskeydown(bool& output) const override {
    if (ex_button == PointingButton::NONE) return false;

    output = ex_isbuttondown;
    return true;
  }

  static void log(bool isCaught, Buttons buttons, int dx, int dy) {
    IOLOG_DEBUG_POINTING("RelativePointerEventCallBack [%7s]: buttons: 0x%08x, dx: %3d, dy: %3d\n",
                         isCaught ? "caught" : "sending",
                         buttons.get(), dx, dy);
  }

  const Buttons buttons;
  const int dx;
  const int dy;

  // Hardware RelativePointerEvent will be split into ButtonEvent and PointerMove.
  // (in EventInputQueue::push_RelativePointerEventCallback.)
  //
  // ex_button is justPressed/justReleased PointingButton.
  // ex_isbuttondown indicates "pressed" or "released".
  const PointingButton ex_button;
  const bool ex_isbuttondown;
};

class Params_ScrollWheelEventCallback final : public Params_Base {
  friend class EventOutputQueue;

public:
  Params_ScrollWheelEventCallback(short da1, short da2, short da3,
                                  IOFixed fd1, IOFixed fd2, IOFixed fd3,
                                  SInt32 pd1, SInt32 pd2, SInt32 pd3,
                                  SInt32 op) : deltaAxis1(da1), deltaAxis2(da2), deltaAxis3(da3),
                                               fixedDelta1(fd1), fixedDelta2(fd2), fixedDelta3(fd3),
                                               pointDelta1(pd1), pointDelta2(pd2), pointDelta3(pd3),
                                               options(op) {}

  ~Params_ScrollWheelEventCallback(void) {}

  const Params_ScrollWheelEventCallback* get_Params_ScrollWheelEventCallback(void) const override { return this; }

  static void log(bool isCaught,
                  short deltaAxis1,
                  short deltaAxis2,
                  short deltaAxis3,
                  IOFixed fixedDelta1,
                  IOFixed fixedDelta2,
                  IOFixed fixedDelta3,
                  SInt32 pointDelta1,
                  SInt32 pointDelta2,
                  SInt32 pointDelta3,
                  SInt32 options) {
    IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%d,%d,%d), pointDelta(%d,%d,%d), options: %d\n",
                         isCaught ? "caught" : "sending",
                         deltaAxis1, deltaAxis2, deltaAxis3,
                         fixedDelta1, fixedDelta2, fixedDelta3,
                         pointDelta1, pointDelta2, pointDelta3,
                         options);
  }

  const short deltaAxis1;
  const short deltaAxis2;
  const short deltaAxis3;
  const IOFixed fixedDelta1;
  const IOFixed fixedDelta2;
  const IOFixed fixedDelta3;
  const SInt32 pointDelta1;
  const SInt32 pointDelta2;
  const SInt32 pointDelta3;
  const SInt32 options;
};

// This params is virtual parameter for EventOutputQueue.
// Hardware does not send this event.
class Params_Wait final : public Params_Base {
  friend class EventOutputQueue;

public:
  Params_Wait(int ms) : milliseconds(ms) {}

  ~Params_Wait(void) {}

  const Params_Wait* get_Params_Wait(void) const override { return this; }

  const int milliseconds;
};

// =================================================
class Params_Factory final {
public:
  static Params_Base* copy(const Params_Base& params) {
    {
      auto p = params.get_Params_KeyboardEventCallBack();
      if (p) {
        return new Params_KeyboardEventCallBack(*p);
      }
    }
    {
      auto p = params.get_Params_UpdateEventFlagsCallback();
      if (p) {
        return new Params_UpdateEventFlagsCallback(*p);
      }
    }
    {
      auto p = params.get_Params_KeyboardSpecialEventCallback();
      if (p) {
        return new Params_KeyboardSpecialEventCallback(*p);
      }
    }
    {
      auto p = params.get_Params_RelativePointerEventCallback();
      if (p) {
        return new Params_RelativePointerEventCallback(*p);
      }
    }
    {
      auto p = params.get_Params_ScrollWheelEventCallback();
      if (p) {
        return new Params_ScrollWheelEventCallback(*p);
      }
    }
    {
      auto p = params.get_Params_Wait();
      if (p) {
        return new Params_Wait(*p);
      }
    }

    return nullptr;
  }
};
}

#endif
