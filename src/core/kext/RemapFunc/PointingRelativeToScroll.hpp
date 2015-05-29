#ifndef POINTINGRELATIVETOSCROLL_HPP
#define POINTINGRELATIVETOSCROLL_HPP

#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"
#include "List.hpp"
#include "RemapFuncBase.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class PointingRelativeToScroll final : public RemapFuncBase {
public:
  static void static_initialize(IOWorkLoop& workloop);
  static void static_terminate(void);
  static void cancelScroll(void);

  PointingRelativeToScroll(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL, autogenId),
                                                  index_(0),
                                                  index_type_(INDEX_TYPE_DEFAULT),
                                                  keytokey_(autogenId),
                                                  isToKeysDefined_(false),
                                                  absolute_distance_(0),
                                                  chained_delta1_(0),
                                                  chained_delta2_(0),
                                                  fixation_delta1_(0),
                                                  fixation_delta2_(0) {
    keytokey_.add(KeyCode::VK_PSEUDO_KEY);
  }

  bool remap(RemapParams& remapParams) override;

  // ----------------------------------------
  // [0] => fromFlags_
  // [0] => fromButtons_
  //
  // [1] => Option::POINTINGRELATIVETOSCROLL_TOFLAGS
  // [2] => toFlags_
  void add(AddDataType datatype, AddValue newval) override;

private:
  enum {
    SCROLL_INTERVAL_MS = 10,
  };

  class Item final : public List::Item {
  public:
    Item(int d1, int d2) : delta1(d1), delta2(d2) {}
    virtual ~Item(void) {}

    int delta1;
    int delta2;
  };

  void toscroll(RemapParams& remapParams);
  static unsigned int abs(int v) { return v > 0 ? v : -v; }
  static unsigned int absmax(int v1, int v2) {
    int a1 = abs(v1);
    int a2 = abs(v2);
    return (a1 > a2) ? a1 : a2;
  }
  static unsigned int absmin(int v1, int v2) {
    int a1 = abs(v1);
    int a2 = abs(v2);
    return (a1 > a2) ? a2 : a1;
  }

  static void timer_callback(OSObject* owner, IOTimerEventSource* sender);

  // ----------
  size_t index_;
  enum {
    INDEX_TYPE_DEFAULT,
    INDEX_TYPE_TOFLAGS,
    INDEX_TYPE_TOKEYS,
  } index_type_;
  FromEvent fromEvent_;
  Vector_ModifierFlag fromModifierFlags_;
  Vector_ModifierFlag toModifierFlags_;
  ToEvent toEvent_;
  KeyToKey keytokey_;
  bool isToKeysDefined_;

  // ----------
  unsigned int absolute_distance_;
  IntervalChecker begin_ic_;

  IntervalChecker chained_ic_;
  int chained_delta1_;
  int chained_delta2_;

  IntervalChecker fixation_ic_;
  IntervalChecker fixation_begin_ic_;
  int fixation_delta1_;
  int fixation_delta2_;

  // ----------
  static List queue_;
  static Vector_ModifierFlag currentFromModifierFlags_;
  static Vector_ModifierFlag currentToModifierFlags_;
  static AutogenId currentAutogenId_;
  static PhysicalEventType lastPhysicalEventType_;
  static TimerWrapper timer_;
};
}
}

#endif
