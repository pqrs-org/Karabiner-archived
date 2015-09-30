#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "CommonData.hpp"
#include "EventInputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCodeModifierFlagPairs.hpp"
#include "KeyboardRepeat.hpp"
#include "ModifierName.hpp"
#include "RemapClass.hpp"
#include "RemapFilter/RemapFilterFactory.hpp"
#include "RemapFunc/RemapFuncFactory.hpp"
#include "VirtualKey/VK_CONFIG.hpp"
#include "VirtualKey/VK_DEFINED_IN_USERSPACE.hpp"
#include "VirtualKey/VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT.hpp"
#include "bridge.h"
#include "strlcpy_utf8.hpp"

namespace org_pqrs_Karabiner {
List RemapClass::ActiveItems::list_;

RemapClass::Item::Item(const RemapClass& parent, const uint32_t* vec, size_t length) : parent_(parent),
                                                                                       type_(BRIDGE_REMAPTYPE_NONE) {
  auto autogenId = AutogenId((static_cast<uint64_t>(parent_.configindex_) << 32) | (parent.items_.size() + 1));
  processor_ = RemapFunc::RemapFuncFactory::create(vec, length, autogenId);
}

RemapClass::Item::~Item(void) {
  ActiveItems::erase_all(this);

  if (processor_) {
    delete processor_;
    processor_ = nullptr;
  }

  // ------------------------------------------------------------
  for (size_t i = 0; i < filters_.size(); ++i) {
    RemapFilter::RemapFilterBase* p = filters_[i];
    if (p) {
      delete p;
    }
  }
}

void RemapClass::Item::append_filter(const unsigned int* vec, size_t length) {
  // ------------------------------------------------------------
  // check parameters.
  //
  if (!vec || length <= 0) {
    IOLOG_ERROR("RemapClass::Item::append_filter invalid parameter.\n");
    return;
  }

  // ------------------------------------------------------------
  // append to filters_.
  //
  RemapFilter::RemapFilterBase* newp = RemapFilter::RemapFilterFactory::create(vec, length);
  if (!newp) {
    IOLOG_ERROR("RemapClass::Item::append_filter new filter is nullptr.\n");
    return;
  }

  filters_.push_back(newp);
}

void RemapClass::Item::remap(RemapParams& remapParams, bool passThroughEnabled) {
  if (!processor_) return;

  bool iskeydown = false;
  bool isalwayskeydown = false;
  if (!remapParams.paramsBase.iskeydown(iskeydown)) {
    iskeydown = true;
    isalwayskeydown = true;
  }

  if (iskeydown) {
    if (passThroughEnabled && !isIgnorePassThrough()) return;
    if (!parent_.enabled()) return;
    if (isblocked()) return;
  } else {
    // We ignore event if `this` is not processed at KeyDown.
    if (!ActiveItems::find(this, ActiveItems::Item::Type::NORMAL)) return;
    if (isblocked_keyup()) return;
  }

  if (!processor_->remap(remapParams)) {
    return;
  }

  if (!isalwayskeydown) {
    if (iskeydown) {
      ActiveItems::push_back(this, ActiveItems::Item::Type::NORMAL);
    } else {
      ActiveItems::erase(this, ActiveItems::Item::Type::NORMAL);
    }
  }
}

void RemapClass::Item::cancelEventOutputQueueItems(bool passThroughEnabled) {
  if (!processor_) return;

  if (passThroughEnabled && !isIgnorePassThrough()) return;

  for (EventOutputQueue::Item* p = static_cast<EventOutputQueue::Item*>(EventOutputQueue::getQueue().safe_front());
       p;
       p = static_cast<EventOutputQueue::Item*>(p->getnext())) {
    auto& paramsBase = p->getParamsBase();
    bool iskeydown = false;
    if (!paramsBase.iskeydown(iskeydown)) {
      iskeydown = true;
    }

    if (iskeydown && !paramsBase.isRepeat()) {
      if (!parent_.enabled()) continue;
      if (isblocked()) continue;
    } else {
      if (isblocked_keyup()) continue;
    }

    processor_->cancelEventOutputQueueItems(*p);
  }
}

bool RemapClass::Item::isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase, bool passThroughEnabled) {
  if (!processor_) return false;

  // BlockUntilKeyUp does not use Flags.
  // So, we do not need to use "active_" flag.

  if (passThroughEnabled && !isIgnorePassThrough()) return false;
  if (!parent_.enabled()) return false;
  if (isblocked()) return false;

  const FromEvent* fromEvent = processor_->getBlockUntilKeyUpFromEvent();
  if (fromEvent) {
    if (fromEvent->isTargetDownEvent(paramsBase) ||
        fromEvent->isTargetUpEvent(paramsBase)) {
      return true;
    }
  }

  return false;
}

bool RemapClass::Item::remap_SimultaneousKeyPresses(bool iskeydown, bool passThroughEnabled) {
  if (!processor_) return false;

  if (iskeydown) {
    if (passThroughEnabled && !isIgnorePassThrough()) return false;
    if (!parent_.enabled()) return false;
    if (isblocked()) return false;
  } else {
    // We ignore event if `this` is not processed at KeyDown.
    if (!ActiveItems::find(this, ActiveItems::Item::Type::SIMULTANEOUSBUTTONPRESSES)) return false;
    if (isblocked_keyup()) return false;
  }

  auto result = processor_->remapSimultaneousKeyPresses();
  if (result == RemapSimultaneousKeyPressesResult::NOT_CHANGED) {
    return false;
  }

  if (result == RemapSimultaneousKeyPressesResult::APPLIED) {
    if (iskeydown) {
      ActiveItems::push_back(this, ActiveItems::Item::Type::SIMULTANEOUSBUTTONPRESSES);
    } else {
      ActiveItems::erase(this, ActiveItems::Item::Type::SIMULTANEOUSBUTTONPRESSES);
    }
  }
  return true;
}

void RemapClass::Item::remap_setkeyboardtype(KeyboardType& keyboardType, bool passThroughEnabled) {
  if (!processor_) return;
  if (passThroughEnabled && !isIgnorePassThrough()) return;
  if (isblocked()) return;
  if (!parent_.enabled()) return;

  processor_->remapSetKeyboardType(keyboardType);
}

void RemapClass::Item::remap_forcenumlockon(ListHookedKeyboard::Item* item, bool passThroughEnabled) {
  if (!processor_) return;
  if (passThroughEnabled && !isIgnorePassThrough()) return;
  if (isblocked()) return;
  if (!parent_.enabled()) return;

  processor_->remapForceNumLockOn(item);
}

bool RemapClass::Item::isPassThroughEnabled(void) const {
  if (!processor_) return false;
  if (isblocked()) return false;
  if (!parent_.enabled()) return false;

  if (processor_->getType() != BRIDGE_REMAPTYPE_PASSTHROUGH) return false;

  return true;
}

bool RemapClass::Item::isblocked(void) const {
  for (size_t i = 0; i < filters_.size(); ++i) {
    RemapFilter::RemapFilterBase* p = filters_[i];
    if (p && p->isblocked()) return true;
  }

  return false;
}

bool RemapClass::Item::isblocked_keyup(void) const {
  for (size_t i = 0; i < filters_.size(); ++i) {
    RemapFilter::RemapFilterBase* p = filters_[i];
    if (p && p->isblocked_keyup()) return true;
  }

  return false;
}

// ----------------------------------------------------------------------
int RemapClass::allocation_count_ = 0;

RemapClass::RemapClass(const uint32_t* const initialize_vector, uint32_t vector_size, uint32_t configindex) : statusmessage_(nullptr),
                                                                                                              enabled_(false),
                                                                                                              is_simultaneouskeypresses_(false),
                                                                                                              configindex_(configindex) {
  if (!initialize_vector) {
    IOLOG_ERROR("RemapClass::RemapClass invalid parameter.\n");
    return;
  }

  // ------------------------------------------------------------
  if (allocation_count_ + vector_size > MAX_ALLOCATION_COUNT) {
    IOLOG_ERROR("RemapClass::RemapClass too many allocation_count_.\n");
    return;
  }
  allocation_count_ += vector_size;

  // ------------------------------------------------------------
  // initialize items_ from vector
  const uint32_t* p = initialize_vector;

  for (;;) {
    if (p == initialize_vector + vector_size) break;

    if (p > initialize_vector + vector_size) {
      IOLOG_ERROR("RemapClass::RemapClass vector_size mismatch. (vector_size:%d)\n", vector_size);
      return;
    }

    uint32_t size = *p++;
    if (p + size > initialize_vector + vector_size) {
      IOLOG_ERROR("RemapClass::RemapClass vector_size mismatch. (vector_size:%d)\n", vector_size);
      return;
    }

    if (size > 0) {
      unsigned int type = p[0];

      if (BRIDGE_REMAPTYPE_NONE < type && type < BRIDGE_REMAPTYPE_END) {
        Item* newp = new Item(*this, p, size);
        if (!newp) {
          IOLOG_ERROR("RemapClass::RemapClass newp == nullptr.\n");
          return;
        }
        items_.push_back(newp);

        if (type == BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES) {
          is_simultaneouskeypresses_ = true;
        }

      } else if (BRIDGE_FILTERTYPE_NONE < type && type < BRIDGE_FILTERTYPE_END) {
        if (items_.size() == 0) {
          IOLOG_ERROR("RemapClass::RemapClass invalid filter (%d).\n", type);
          return;
        }
        Item* back = items_.back();
        if (back) {
          back->append_filter(p, size);
        }

      } else if (type == BRIDGE_STATUSMESSAGE) {
        size_t length = BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN;
        if (!statusmessage_) {
          statusmessage_ = new char[length];
        }
        pqrs::strlcpy_utf8::strlcpy(statusmessage_, reinterpret_cast<const char*>(p + 1), length);

      } else if (type == BRIDGE_MODIFIERNAME) {
        if (size < 3) {
          IOLOG_ERROR("RemapClass::RemapClass invalid size for BRIDGE_MODIFIERNAME. (%d)\n", size);
          return;
        } else {
          unsigned int modifierFlag = p[1];
          ModifierName::registerVirtualModifier(ModifierFlag(modifierFlag), reinterpret_cast<const char*>(p + 2));
        }

      } else if (type == BRIDGE_VK_MODIFIER) {
        if (size != 12) {
          IOLOG_ERROR("RemapClass::RemapClass invalid size for BRIDGE_VK_MODIFIER. (%d)\n", size);
          return;
        } else {
          KeyCodeModifierFlagPairs::registerVirtualModifier(ModifierFlag(p[1]),
                                                            KeyCode(p[2]),   // VK_MODIFIER_*
                                                            KeyCode(p[3]),   // VK_LOCK_*
                                                            KeyCode(p[4]),   // VK_LOCK_*_FORCE_ON
                                                            KeyCode(p[5]),   // VK_LOCK_*_FORCE_OFF
                                                            KeyCode(p[6]),   // VK_NEGATIVE_LOCK_*
                                                            KeyCode(p[7]),   // VK_NEGATIVE_LOCK_*_FORCE_ON
                                                            KeyCode(p[8]),   // VK_NEGATIVE_LOCK_*_FORCE_OFF
                                                            KeyCode(p[9]),   // VK_STICKY_*
                                                            KeyCode(p[10]),  // VK_STICKY_*_FORCE_ON
                                                            KeyCode(p[11])); // VK_STICKY_*_FORCE_OFF
        }

      } else if (type == BRIDGE_VK_CONFIG) {
        if (size != 5) {
          IOLOG_ERROR("RemapClass::RemapClass invalid size for BRIDGE_VK_CONFIG. (%d)\n", size);
          return;

        } else {
          unsigned int keycode_toggle = p[1];
          unsigned int keycode_force_on = p[2];
          unsigned int keycode_force_off = p[3];
          unsigned int keycode_sync_keydownup = p[4];
          VirtualKey::VK_CONFIG::add_item(this,
                                          keycode_toggle,
                                          keycode_force_on,
                                          keycode_force_off,
                                          keycode_sync_keydownup);
        }

      } else if (type == BRIDGE_VK_CHANGE_INPUTSOURCE || type == BRIDGE_VK_OPEN_URL) {
        if (size != 2) {
          IOLOG_ERROR("RemapClass::RemapClass invalid size for VK_DEFINED_IN_USERSPACE (type:%d, %d)\n", type, size);
          return;

        } else {
          unsigned int keycode = p[1];
          uint32_t notification_type = 0;
          switch (type) {
          case BRIDGE_VK_CHANGE_INPUTSOURCE:
            notification_type = BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE;
            break;
          case BRIDGE_VK_OPEN_URL:
            notification_type = BRIDGE_USERCLIENT_NOTIFICATION_TYPE_OPEN_URL;
            break;
          default:
            IOLOG_ERROR("RemapClass::RemapClass invalid type for VK_DEFINED_IN_USERSPACE (type:%d)\n", type);
            return;
          }
          VirtualKey::VK_DEFINED_IN_USERSPACE::add_item(this, keycode, notification_type);
        }

      } else {
        IOLOG_ERROR("RemapClass::RemapClass unknown type:%d.\n", type);
        return;
      }

      p += size;
    }
  }
}

RemapClass::~RemapClass(void) {
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p) {
      delete p;
    }
  }
  if (statusmessage_) {
    delete[] statusmessage_;
  }
}

void RemapClass::remap_setkeyboardtype(KeyboardType& keyboardType, bool passThroughEnabled) {
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p) {
      p->remap_setkeyboardtype(keyboardType, passThroughEnabled);
    }
  }
}

void RemapClass::remap_forcenumlockon(ListHookedKeyboard::Item* item, bool passThroughEnabled) {
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p) {
      p->remap_forcenumlockon(item, passThroughEnabled);
    }
  }
}

void RemapClass::remap(RemapParams& remapParams, bool passThroughEnabled) {
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p) {
      // DependingPressingPeriodKeyToKey watches another key status.
      // Therefore, we need to call 'p->remap(remapParams)' for all items.
      p->remap(remapParams, passThroughEnabled);
    }
  }
}

bool RemapClass::isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase, bool passThroughEnabled) {
  bool isTargetEvent = false;

  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p) {
      if (p->isTargetEventForBlockUntilKeyUp(paramsBase, passThroughEnabled)) {
        isTargetEvent = true;
      }
    }
  }

  return isTargetEvent;
}

bool RemapClass::remap_simultaneouskeypresses(bool iskeydown, bool passThroughEnabled) {
  bool queue_changed = false;
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p) {
      if (p->remap_SimultaneousKeyPresses(iskeydown, passThroughEnabled)) {
        queue_changed = true;
      }
    }
  }

  return queue_changed;
}

void RemapClass::cancelEventOutputQueueItems(bool passThroughEnabled) {
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p) {
      p->cancelEventOutputQueueItems(passThroughEnabled);
    }
  }
}

bool RemapClass::hasActiveItem(void) const {
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p && p->active()) return true;
  }
  return false;
}

bool RemapClass::isPassThroughEnabled(void) const {
  for (size_t i = 0; i < items_.size(); ++i) {
    Item* p = items_[i];
    if (p && p->isPassThroughEnabled()) return true;
  }
  return false;
}

void RemapClass::log_allocation_count(void) {
  IOLOG_INFO("RemapClass::allocation_count_ %d/%d (memory usage: %d%% of %dKB)\n",
             allocation_count_,
             MAX_ALLOCATION_COUNT,
             allocation_count_ * 100 / MAX_ALLOCATION_COUNT,
             static_cast<int>(MAX_ALLOCATION_COUNT * sizeof(uint32_t) / 1024));
}

void RemapClass::reset_allocation_count(void) {
  allocation_count_ = 0;
}

// ================================================================================
namespace RemapClassManager {
TimerWrapper refresh_timer_;

char statusmessage_[BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN];
char lastmessage_[BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN];
bool isSimultaneousKeyPressesEnabled_ = false;

Vector_RemapClassPointer remapclasses_;
Vector_RemapClassPointer enabled_remapclasses_;

List prepareTargetItems_;

// ======================================================================
static void
refresh_timer_callback(OSObject* owner, IOTimerEventSource* sender) {
  enabled_remapclasses_.clear();

  // ----------------------------------------
  statusmessage_[0] = '\0';

  isSimultaneousKeyPressesEnabled_ = false;

  for (size_t i = 0; i < remapclasses_.size(); ++i) {
    RemapClass* p = remapclasses_[i];
    if (!p) continue;

    if (p->enabled() || p->hasActiveItem()) {
      enabled_remapclasses_.push_back(p);

      if (p->enabled()) {
        const char* msg = p->get_statusmessage();
        if (msg) {
          strlcat(statusmessage_, msg, sizeof(statusmessage_));
          strlcat(statusmessage_, " ", sizeof(statusmessage_));
        }

        if (p->is_simultaneouskeypresses()) {
          isSimultaneousKeyPressesEnabled_ = true;
        }
      }
    }
  }

  if (strcmp(statusmessage_, lastmessage_) != 0) {
    pqrs::strlcpy_utf8::strlcpy(lastmessage_, statusmessage_, sizeof(lastmessage_));

    int index = BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA;
    CommonData::clear_statusmessage(index);
    CommonData::append_statusmessage(index, statusmessage_);
    CommonData::send_notification_statusmessage(index);
  }
}

// ======================================================================

void initialize(IOWorkLoop& workloop) {
  statusmessage_[0] = '\0';
  lastmessage_[0] = '\0';

  refresh_timer_.initialize(&workloop, nullptr, refresh_timer_callback);
}

static void
clear_remapclasses(void) {
  KeyCodeModifierFlagPairs::clearVirtualModifiers();
  ModifierName::clearVirtualModifiers();
  VirtualKey::VK_CONFIG::clear_items();
  VirtualKey::VK_DEFINED_IN_USERSPACE::clear_items();

  enabled_remapclasses_.clear();
  prepareTargetItems_.clear();

  for (size_t i = 0; i < remapclasses_.size(); ++i) {
    RemapClass* p = remapclasses_[i];
    if (p) {
      delete p;
    }
  }
  remapclasses_.clear();
}

void terminate(void) {
  refresh_timer_.terminate();

  clear_remapclasses();
}

bool load_remapclasses_initialize_vector(const uint32_t* const remapclasses_initialize_vector, mach_vm_size_t vector_size) {
  // ------------------------------------------------------------
  // clean previous resources and setup new resources.
  clear_remapclasses();

  // ------------------------------------------------------------
  // Validate vector_size

  // "vector_size" is byte of remapclasses_initialize_vector. (!= count of items.)
  // Confirming vector_size is a multiple of sizeof(uint32_t).
  if ((vector_size % sizeof(uint32_t)) != 0) {
    IOLOG_ERROR("%s (vector_size %% sizeof(uint32_t)) != 0. (%d)\n", __FUNCTION__, static_cast<int>(vector_size));
    goto error;
  }

  // change vector_size to num of uint32_t.
  vector_size /= sizeof(uint32_t);
  // Then, we can treat "remapclasses_initialize_vector + vector_size" as valid.

  if (vector_size < 2) {
    IOLOG_ERROR("%s vector_size < 2. (%d)\n", __FUNCTION__, static_cast<int>(vector_size));
    goto error;
  }
  if (vector_size > RemapClass::MAX_ALLOCATION_COUNT) {
    IOLOG_ERROR("%s too large vector_size. (%d)\n", __FUNCTION__, static_cast<int>(vector_size));
    goto error;
  }

  // ------------------------------------------------------------
  {
    const uint32_t* p = remapclasses_initialize_vector;
    uint32_t count = *p++;

    if (count > RemapClass::MAX_CONFIG_COUNT) {
      IOLOG_ERROR("%s too many count. (%d)\n", __FUNCTION__, count);
      goto error;
    }

    // ------------------------------------------------------------
    // load
    remapclasses_.reserve(count);
    RemapClass::reset_allocation_count();

    // (1) Setting nullptr to all items.
    for (uint32_t i = 0; i < count; ++i) {
      remapclasses_.push_back(nullptr);
    }

    // (2) Setting RemapClass* to items.
    for (uint32_t i = 0; i < count; ++i) {
      if (p >= remapclasses_initialize_vector + vector_size) {
        IOLOG_ERROR("%s vector_size mismatch.\n", __FUNCTION__);
        goto error;
      }

      uint32_t size = *p++;
      if (p + size > remapclasses_initialize_vector + vector_size) {
        IOLOG_ERROR("%s vector_size mismatch. (vector_size:%d, size:%d)\n", __FUNCTION__,
                    static_cast<int>(vector_size), size);
        goto error;
      }
      if (size == 0) {
        IOLOG_ERROR("%s size == 0.\n", __FUNCTION__);
        goto error;
      }

      uint32_t configindex = *p++;
      --size;

      if (configindex >= remapclasses_.size()) {
        IOLOG_ERROR("%s invalid configindex %d (remapclasses_.size() == %d).\n", __FUNCTION__,
                    configindex, static_cast<int>(remapclasses_.size()));
        goto error;
      }

      RemapClass* newp = new RemapClass(p, size, configindex);
      if (!newp) {
        IOLOG_ERROR("%s newp == nullptr.\n", __FUNCTION__);
        goto error;
      }
      p += size;

      remapclasses_[configindex] = newp;
    }

    // (3) Making sure that is not nullptr for all items.
    for (uint32_t i = 0; i < remapclasses_.size(); ++i) {
      if (!remapclasses_[i]) {
        IOLOG_ERROR("%s remapclasses_[i] == nullptr.\n", __FUNCTION__);
        goto error;
      }
    }
  }

  RemapClass::log_allocation_count();

  return true;

error:
  clear_remapclasses();
  return false;
}

bool set_config(const int32_t* const config_vector, mach_vm_size_t config_size) {
  // ------------------------------------------------------------
  // check
  if (config_size != (BRIDGE_ESSENTIAL_CONFIG_INDEX__END__ + remapclasses_.size()) * sizeof(int32_t)) {
    IOLOG_ERROR("%s config_size mismatch.\n", __FUNCTION__);
    return false;
  }

  // ------------------------------------------------------------
  // essential config
  const int32_t* p = config_vector;
  Config::set_essential_config(p, BRIDGE_ESSENTIAL_CONFIG_INDEX__END__);
  // remapclasses config
  p += BRIDGE_ESSENTIAL_CONFIG_INDEX__END__;
  for (size_t i = 0; i < remapclasses_.size(); ++i) {
    RemapClass* rc = remapclasses_[i];
    if (!rc) {
      IOLOG_ERROR("%s RemapClass == nullptr.\n", __FUNCTION__);
    } else {
      rc->setEnabled(p[i]);
    }
  }

  refresh();

  return true;
}

bool set_config_one(bool isEssentialConfig, uint32_t index, int32_t value) {
  bool succeed = false;

  if (isEssentialConfig) {
    succeed = Config::set_essential_config_one(index, value);
  } else {
    if (index >= remapclasses_.size()) {
      IOLOG_ERROR("%s index is invalid.\n", __FUNCTION__);
    } else {
      RemapClass* rc = remapclasses_[index];
      if (!rc) {
        IOLOG_ERROR("%s RemapClass == nullptr.\n", __FUNCTION__);
      } else {
        rc->setEnabled(value);
        succeed = true;
      }
    }
  }

  refresh();

  return succeed;
}

void refresh(void) {
  // We use timer to prevent deadlock of lock_. (refresh may be called in the "remap" method.)
  refresh_timer_.setTimeoutMS(0);
}

// ----------------------------------------------------------------------
static bool
isPassThroughEnabled(void) {
  for (size_t i = 0; i < enabled_remapclasses_.size(); ++i) {
    RemapClass* p = enabled_remapclasses_[i];
    if (p && p->isPassThroughEnabled()) return true;
  }
  return false;
}

#define CALL_REMAPCLASS_FUNC(FUNC, PARAMS)                      \
  {                                                             \
    for (size_t i = 0; i < enabled_remapclasses_.size(); ++i) { \
      RemapClass* p = enabled_remapclasses_[i];                 \
      if (p) p->FUNC(PARAMS, passThroughEnabled);               \
    }                                                           \
  }

void remap_setkeyboardtype(KeyboardType& keyboardType) {
  bool passThroughEnabled = isPassThroughEnabled();
  CALL_REMAPCLASS_FUNC(remap_setkeyboardtype, keyboardType);
}

void remap_forcenumlockon(ListHookedKeyboard::Item* item) {
  bool passThroughEnabled = isPassThroughEnabled();
  CALL_REMAPCLASS_FUNC(remap_forcenumlockon, item);
}

void remap(RemapParams& remapParams) {
  bool passThroughEnabled = isPassThroughEnabled();
  CALL_REMAPCLASS_FUNC(remap, remapParams);
}

#undef CALL_REMAPCLASS_FUNC

bool isTargetEventForBlockUntilKeyUp(const Params_Base& paramsBase) {
  bool passThroughEnabled = isPassThroughEnabled();
  bool isTargetEvent = false;

  for (size_t i = 0; i < enabled_remapclasses_.size(); ++i) {
    RemapClass* p = enabled_remapclasses_[i];
    if (p) {
      if (p->isTargetEventForBlockUntilKeyUp(paramsBase, passThroughEnabled)) {
        isTargetEvent = true;
      }
    }
  }

  return isTargetEvent;
}

bool remap_simultaneouskeypresses(bool iskeydown) {
  bool passThroughEnabled = isPassThroughEnabled();
  bool queue_changed = false;

  for (size_t i = 0; i < enabled_remapclasses_.size(); ++i) {
    RemapClass* p = enabled_remapclasses_[i];
    if (p) {
      if (p->remap_simultaneouskeypresses(iskeydown, passThroughEnabled)) {
        queue_changed = true;
      }
    }
  }

  return queue_changed;
}

void cancelEventOutputQueueItems(void) {
  bool passThroughEnabled = isPassThroughEnabled();

  for (size_t i = 0; i < enabled_remapclasses_.size(); ++i) {
    RemapClass* p = enabled_remapclasses_[i];
    if (p) {
      p->cancelEventOutputQueueItems(passThroughEnabled);
    }
  }
}

bool isSimultaneousKeyPressesEnabled(void) {
  return isSimultaneousKeyPressesEnabled_;
}

bool isEnabled(size_t configindex) {
  if (configindex >= remapclasses_.size()) {
    IOLOG_ERROR("RemapClass::isEnabled invalid configindex.\n");
    return false;
  }

  RemapClass* p = remapclasses_[configindex];
  if (!p) return false;

  return p->enabled();
}

void registerPrepareTargetItem(RemapFunc::RemapFuncBase* processor) {
  unregisterPrepareTargetItem(processor);
  if (processor) {
    prepareTargetItems_.push_back(new PrepareTargetItem(processor));
  }
}

void unregisterPrepareTargetItem(RemapFunc::RemapFuncBase* processor) {
  auto item = static_cast<PrepareTargetItem*>(prepareTargetItems_.safe_front());
  for (;;) {
    if (!item) return;

    if ((item->remapFuncBaseWeakPointer).expired() ||
        (item->remapFuncBaseWeakPointer).get() == processor) {
      item = static_cast<PrepareTargetItem*>(prepareTargetItems_.erase_and_delete(item));
    } else {
      item = static_cast<PrepareTargetItem*>(item->getnext());
    }
  }
}

void prepare(RemapParams& remapParams) {
  // Decrease EventInputQueue::serialNumber_ in order to avoid event cancellation by cancelEventOutputQueueItems.
  EventInputQueue::ScopedSerialNumberDecreaser scopedSerialNumberDecreaser;

  // VirtualKey::VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT::needToDrop will be set in KeyToKey::prepare.
  // If needToDrop is set in prepare, we should ignore this event.
  // So, set remapParams.isremapped true when needToDrop is set.
  VirtualKey::VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT::setNeedToDrop(false);

  auto item = static_cast<PrepareTargetItem*>(prepareTargetItems_.safe_front());
  for (;;) {
    if (!item) break;

    // item might be removed by unregisterPrepareTargetItem in `prepare`.
    // So, we store next pointer at first.
    auto next = static_cast<PrepareTargetItem*>(item->getnext());

    if (!item->remapFuncBaseWeakPointer.expired()) {
      item->remapFuncBaseWeakPointer->prepare(remapParams);
    }

    item = next;
  }

  if (VirtualKey::VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT::needToDrop()) {
    remapParams.isremapped = true;
  }
}
}
}
