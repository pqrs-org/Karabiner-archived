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

  class Item;

  class ActiveItems final {
    friend class Item;

  public:
    class Item final : public List::Item {
    public:
      enum class Type {
        NORMAL,
        SIMULTANEOUSBUTTONPRESSES,
      };

      Item(const RemapClass::Item* p, Type t) : item_(p), type_(t) {}
      ~Item(void) {}

      const RemapClass::Item* getItem(void) const { return item_; }
      Type getType(void) const { return type_; }

    private:
      const RemapClass::Item* item_;
      Type type_;
    };

    static Item* find(const RemapClass::Item* p, Item::Type t) {
      for (Item* q = static_cast<Item*>(list_.safe_front()); q; q = static_cast<Item*>(q->getnext())) {
        if (q->getItem() == p && q->getType() == t) {
          return q;
        }
      }
      return nullptr;
    }

    static void clear(void) {
      list_.clear();
    }

    static void push_back(const RemapClass::Item* p, Item::Type t) {
      list_.push_back(new Item(p, t));
      if (list_.size() > 60000) {
        IOLOG_WARN("RemapClass::ActiveItems too many item count: %ld\n", list_.size());
      }
    }

    static bool erase(const RemapClass::Item* p, Item::Type t) {
      Item* q = find(p, t);
      if (q) {
        list_.erase_and_delete(q);
        return true;
      }
      return false;
    }

    static void erase_all(const RemapClass::Item* p) {
      Item* q = static_cast<Item*>(list_.safe_front());
      while (q) {
        if (q->getItem() == p) {
          q = static_cast<Item*>(list_.erase_and_delete(q));
        } else {
          q = static_cast<Item*>(q->getnext());
        }
      }
    }

  private:
    static List list_;
  };

  class Item final {
  public:
    Item(const RemapClass& parent, const uint32_t* vec, size_t length);
    ~Item(void);
    void append_filter(const uint32_t* vec, size_t length);

    // --------------------
    void remap(RemapParams& remapParams, bool passThroughEnabled);
    // for DropAllKeys, DropKeyAfterRemap
    void cancelEventOutputQueueItems(bool passThroughEnabled);
    // for BlockUntilKeyUp
    bool isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase, bool passThroughEnabled);
    //
    bool remap_SimultaneousKeyPresses(bool iskeydown, bool passThroughEnabled);
    //
    void remap_setkeyboardtype(KeyboardType& keyboardType, bool passThroughEnabled);
    //
    void remap_forcenumlockon(ListHookedKeyboard::Item* item, bool passThroughEnabled);

    bool active(void) const {
      return (ActiveItems::find(this, ActiveItems::Item::Type::NORMAL) ||
              ActiveItems::find(this, ActiveItems::Item::Type::SIMULTANEOUSBUTTONPRESSES));
    }

    bool isPassThroughEnabled(void) const;

  private:
    bool isblocked(void) const;
    bool isblocked_keyup(void) const;

    Item(const Item& rhs);            // Prevent copy-construction
    Item& operator=(const Item& rhs); // Prevent assignment

    bool isIgnorePassThrough(void) const {
      if (!processor_) return false;
      return processor_->getIgnorePassThrough();
    }

    const RemapClass& parent_;

    uint32_t type_;

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
  void remap(RemapParams& remapParams, bool passThroughEnabled);

  // for BlockUntilKeyUp
  bool isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase, bool passThroughEnabled);
  bool remap_simultaneouskeypresses(bool iskeydown, bool passThroughEnabled);
  void cancelEventOutputQueueItems(bool passThroughEnabled);
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

void cancelEventOutputQueueItems(void);

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
