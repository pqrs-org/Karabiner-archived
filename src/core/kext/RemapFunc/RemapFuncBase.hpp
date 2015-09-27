#ifndef REMAPFUNCBASE_HPP
#define REMAPFUNCBASE_HPP

#include "bridge.h"
#include "EventOutputQueue.hpp"
#include "FromEvent.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "ListHookedKeyboard.hpp"
#include "RemapFuncClasses.hpp"
#include "ToEvent.hpp"
#include "WeakPointer.hpp"

namespace org_pqrs_Karabiner {
class RemapSimultaneousKeyPressesResult {
public:
  enum Value {
    NOT_CHANGED,

    // __SimultaneousKeyPresses__ has been applied.
    // And KeyCode::VK_SIMULTANEOUSKEYPRESSES_* has been pushed.
    APPLIED,

    // __SimultaneousKeyPresses__ has been applied.
    // But no KeyCode::VK_SIMULTANEOUSKEYPRESSES_* has been pushed.
    // This result will be returned when key up event and some keys remain pressed.
    //
    // We need to continue remap_simultaneouskeypresses in EventInputQueue but
    // do not change active_SimultaneousButtonPresses_ in RemapClass::Item.
    QUEUE_CHANGED,
  };
};

namespace RemapFunc {
DECLARE_WEAKPOINTER(RemapFuncBase);

class RemapFuncBase {
protected:
  RemapFuncBase(unsigned int type, AutogenId autogenId) : type_(type),
                                                          ignorePassThrough_(false),
                                                          autogenId_(autogenId) {
    WeakPointerManager_RemapFuncBase::add(this);
  }

public:
  virtual ~RemapFuncBase(void) {
    WeakPointerManager_RemapFuncBase::remove(this);
  }

  virtual void add(AddDataType datatype, AddValue newval) = 0;

  // Clean up internal state before `remap`.
  // For example, send short period key event in HoldingKeyToKey.
  virtual void prepare(RemapParams& remapParams) {}

  virtual bool remap(RemapParams& remapParams) { return false; }
  virtual void cancelEventOutputQueueItems(EventOutputQueue::Item& item) {}
  virtual RemapSimultaneousKeyPressesResult::Value remapSimultaneousKeyPresses(void) {
    return RemapSimultaneousKeyPressesResult::NOT_CHANGED;
  }
  virtual bool remapSetKeyboardType(KeyboardType& keyboardType) { return false; }
  virtual bool remapForceNumLockOn(ListHookedKeyboard::Item* item) { return false; }
  virtual const FromEvent* getBlockUntilKeyUpFromEvent(void) const { return nullptr; }

  unsigned int getType(void) const { return type_; }
  void setIgnorePassThrough(bool v) { ignorePassThrough_ = v; }
  bool getIgnorePassThrough(void) const { return ignorePassThrough_; }

private:
  unsigned int type_;
  bool ignorePassThrough_;

protected:
  const AutogenId autogenId_;
};
}
}

#endif
