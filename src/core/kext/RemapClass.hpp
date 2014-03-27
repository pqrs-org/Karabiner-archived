#ifndef REMAPCLASS_HPP
#define REMAPCLASS_HPP

#include "bridge.h"
#include "RemapFilter/FilterUnion.hpp"
#include "RemapFunc/DoublePressModifier.hpp"
#include "RemapFunc/DropKeyAfterRemap.hpp"
#include "RemapFunc/DropPointingRelativeCursorMove.hpp"
#include "RemapFunc/DropScrollWheel.hpp"
#include "RemapFunc/FlipPointingRelative.hpp"
#include "RemapFunc/FlipScrollWheel.hpp"
#include "RemapFunc/ForceNumLockOn.hpp"
#include "RemapFunc/HoldingKeyToKey.hpp"
#include "RemapFunc/IgnoreMultipleSameKeyPress.hpp"
#include "RemapFunc/KeyOverlaidModifier.hpp"
#include "RemapFunc/KeyToKey.hpp"
#include "RemapFunc/KeyUpEventToKey.hpp"
#include "RemapFunc/PassThrough.hpp"
#include "RemapFunc/PointingRelativeToKey.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"
#include "RemapFunc/ScrollWheelToKey.hpp"
#include "RemapFunc/ScrollWheelToScrollWheel.hpp"
#include "RemapFunc/SetKeyboardType.hpp"
#include "RemapFunc/SimultaneousKeyPresses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class RemapClass {
  public:
    enum {
      MAX_CONFIG_COUNT = 10000,
      MAX_ALLOCATION_COUNT = (8 * 1024 * 1024) / sizeof(uint32_t), // 8MB
    };

    class Item {
    public:
      Item(const RemapClass& parent, const uint32_t* vec, size_t length);
      ~Item(void);
      void append_filter(const uint32_t* vec, size_t length);

      // --------------------
      void remap(RemapParams& remapParams);
      // for DropKeyAfterRemap
      bool drop(const Params_KeyboardEventCallBack& params);
      //
      bool remap_SimultaneousKeyPresses(void);
      //
      void remap_setkeyboardtype(KeyboardType& keyboardType);
      //
      void remap_forcenumlockon(ListHookedKeyboard::Item* item);

      bool active(void) const { return active_; }

      bool isPassThroughEnabled(void) const;
      bool isIgnorePassThrough(void) const { return ignorePassThrough_; }

    private:
      bool isblocked(void) const;

      Item(const Item& rhs);
      Item& operator=(const Item& rhs);

      const RemapClass& parent_;

      uint32_t type_;

      // true if remapped at KeyDown.
      bool active_;

      // true if the setting is effective in PassThrough Mode.
      bool ignorePassThrough_;

      union {
        RemapFunc::DoublePressModifier* doublePressModifier;
        RemapFunc::DropKeyAfterRemap* dropKeyAfterRemap;
        RemapFunc::DropPointingRelativeCursorMove* dropPointingRelativeCursorMove;
        RemapFunc::DropScrollWheel* dropScrollWheel;
        RemapFunc::FlipPointingRelative* flipPointingRelative;
        RemapFunc::FlipScrollWheel* flipScrollWheel;
        RemapFunc::ForceNumLockOn* forceNumLockOn;
        RemapFunc::HoldingKeyToKey* holdingKeyToKey;
        RemapFunc::IgnoreMultipleSameKeyPress* ignoreMultipleSameKeyPress;
        RemapFunc::KeyUpEventToKey* keyUpEventToKey;
        RemapFunc::KeyOverlaidModifier* keyOverlaidModifier;
        RemapFunc::KeyToKey* keyToKey;
        RemapFunc::PassThrough* passThrough;
        RemapFunc::PointingRelativeToKey* pointingRelativeToKey;
        RemapFunc::PointingRelativeToScroll* pointingRelativeToScroll;
        RemapFunc::ScrollWheelToKey* scrollWheelToKey;
        RemapFunc::ScrollWheelToScrollWheel* scrollWheelToScrollWheel;
        RemapFunc::SetKeyboardType* setKeyboardType;
        RemapFunc::SimultaneousKeyPresses* simultaneousKeyPresses;
      } p_;

      RemapFilter::Vector_FilterUnionPointer filters_;
    };
    typedef Item* ItemPointer;
    DECLARE_VECTOR(ItemPointer);

    // ----------------------------------------------------------------------
    RemapClass(const uint32_t* const initialize_vector, uint32_t vector_size, uint32_t configindex);
    ~RemapClass(void);

    void remap_setkeyboardtype(KeyboardType& keyboardType, bool passThroughEnabled);
    void remap_forcenumlockon(ListHookedKeyboard::Item* item, bool passThroughEnabled);
    void remap(RemapParams& remapParams, bool passThroughEnabled);

    bool remap_simultaneouskeypresses(bool passThroughEnabled);
    bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params, bool passThroughEnabled);
    const char* get_statusmessage(void) const { return statusmessage_; }
    bool enabled(void) const { return enabled_; }
    void setEnabled(bool newval) { enabled_ = newval; }
    void toggleEnabled(void) { setEnabled(! enabled_); }
    bool is_simultaneouskeypresses(void) const { return is_simultaneouskeypresses_; }
    uint32_t get_configindex(void) const { return configindex_; }
    bool hasActiveItem(void) const;
    bool isPassThroughEnabled(void) const;

    static void log_allocation_count(void);
    static void reset_allocation_count(void);

  private:
    RemapClass(const RemapClass& rhs);
    RemapClass& operator=(const RemapClass& rhs);

    Vector_ItemPointer items_;
    char* statusmessage_;
    bool enabled_;
    bool is_simultaneouskeypresses_;
    uint32_t configindex_;

    static int allocation_count_;
  };
  typedef RemapClass* RemapClassPointer;
  DECLARE_VECTOR(RemapClassPointer);

  // ================================================================================
  namespace RemapClassManager {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);

    bool load_remapclasses_initialize_vector(const uint32_t* const remapclasses_initialize_vector, mach_vm_size_t vector_size);
    bool set_config(const int32_t* const config_vector, mach_vm_size_t config_size);
    bool set_config_one(bool isEssentialConfig, uint32_t index, int32_t value);

    // call after setting enable/disable status is changed.
    // (PreferencesPane, VK_CONFIG)
    void refresh(void);

    void remap_setkeyboardtype(KeyboardType& keyboardType);
    void remap_forcenumlockon(ListHookedKeyboard::Item* item);
    void remap(RemapParams& remapParams);

    // return true if EventInputQueue::queue_ is changed.
    bool remap_simultaneouskeypresses(void);

    // return true if dropped.
    bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params);

    bool isEventInputQueueDelayEnabled(void);

    bool isEnabled(size_t configindex);
  };
}

#endif
