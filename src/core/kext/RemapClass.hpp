#ifndef REMAPCLASS_HPP
#define REMAPCLASS_HPP

#include "bridge.h"
#include "RemapFilter/RemapFilterBase.hpp"
#include "RemapFunc/RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
class RemapClass final {
public:
  enum {
    MAX_CONFIG_COUNT = 10000,
    MAX_ALLOCATION_COUNT = (8 * 1024 * 1024) / sizeof(uint32_t), // 8MB
  };

  class Item final {
  public:
    Item(const RemapClass& parent, const uint32_t* vec, size_t length);
    ~Item(void);
    void append_filter(const uint32_t* vec, size_t length);

    // --------------------
    void prepare(RemapParams& remapParams);
    void remap(RemapParams& remapParams);
    // for DropKeyAfterRemap
    bool drop(const Params_KeyboardEventCallBack& params);
    // for BlockUntilKeyUp
    bool isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase);
    //
    bool remap_SimultaneousKeyPresses(bool iskeydown);
    //
    void remap_setkeyboardtype(KeyboardType& keyboardType);
    //
    void remap_forcenumlockon(ListHookedKeyboard::Item* item);

    bool active(void) const { return active_; }

    bool isPassThroughEnabled(void) const;
    bool isIgnorePassThrough(void) const {
      if (!processor_) return false;
      return processor_->getIgnorePassThrough();
    }

  private:
    bool isblocked(void) const;
    bool isblocked_keyup(void) const;

    Item(const Item& rhs);            // Prevent copy-construction
    Item& operator=(const Item& rhs); // Prevent assignment

    const RemapClass& parent_;

    uint32_t type_;

    // true if remapped at KeyDown.
    bool active_;
    bool active_SimultaneousButtonPresses_;

    RemapFunc::RemapFuncBase* processor_;
    RemapFilter::Vector_RemapFilterBasePointer filters_;
  };
  typedef Item* ItemPointer;
  DECLARE_VECTOR(ItemPointer);

  // ----------------------------------------------------------------------
  RemapClass(const uint32_t* const initialize_vector, uint32_t vector_size, uint32_t configindex);
  ~RemapClass(void);

  void remap_setkeyboardtype(KeyboardType& keyboardType, bool passThroughEnabled);
  void remap_forcenumlockon(ListHookedKeyboard::Item* item, bool passThroughEnabled);
  void prepare(RemapParams& remapParams, bool passThroughEnabled);
  void remap(RemapParams& remapParams, bool passThroughEnabled);

  // for BlockUntilKeyUp
  bool isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase, bool passThroughEnabled);
  bool remap_simultaneouskeypresses(bool iskeydown, bool passThroughEnabled);
  bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params, bool passThroughEnabled);
  const char* get_statusmessage(void) const { return statusmessage_; }
  bool enabled(void) const { return enabled_; }
  void setEnabled(bool newval) { enabled_ = newval; }
  void toggleEnabled(void) { setEnabled(!enabled_); }
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

// for BlockUntilKeyUp
bool isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase);

// return true if EventInputQueue::queue_ is changed.
//
// Implementation Note:
// Do not pass Params_Base to remap_simultaneouskeypresses because
// this method may modify EventInputQueue (pop queue).
// If passed Params_Base is deleted by pop queue,
// access violation will happen.
bool remap_simultaneouskeypresses(bool iskeydown);

// return true if dropped.
bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params);

bool isSimultaneousKeyPressesEnabled(void);

bool isEnabled(size_t configindex);

class PrepareTargetItem final : public List::Item {
public:
  PrepareTargetItem(RemapFunc::RemapFuncBase* p) : remapFuncBaseWeakPointer(p) {}
  virtual ~PrepareTargetItem(void) {}

  RemapFunc::WeakPointer_RemapFuncBase remapFuncBaseWeakPointer;

private:
  PrepareTargetItem& operator=(const PrepareTargetItem& rhs); // Prevent assignment
};

void registerPrepareTargetItem(RemapFunc::RemapFuncBase* processor);
void unregisterPrepareTargetItem(RemapFunc::RemapFuncBase* processor);
void prepare(RemapParams& remapParams);
}
}

#endif
