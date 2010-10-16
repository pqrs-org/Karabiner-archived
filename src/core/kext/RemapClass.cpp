#include "bridge.h"
#include "RemapClass.hpp"
#include "Client.hpp"
#include "KeyboardRepeat.hpp"
#include "util/CommonData.hpp"
#include "util/EventInputQueue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  RemapClass::Item::initialize_remap(const unsigned int* vec, size_t length)
  {
    type_ = BRIDGE_REMAPTYPE_NONE;

    // ------------------------------------------------------------
    // check parameters.
    //
    if (! vec || length <= 0) {
      IOLOG_ERROR("RemapClass::Item::initialize_remap invalid parameter.\n");
      goto error;
    }

    // ------------------------------------------------------------
    // initialize values.
    //
    type_ = vec[0];

#define INITIALIZE_UNION_VALUE(POINTER, CLASS) {                               \
    POINTER = new CLASS;                                                       \
    if (! POINTER) {                                                           \
      IOLOG_ERROR("RemapClass::Item::initialize_remap failed to allocate.\n"); \
      goto error;                                                              \
    } else {                                                                   \
      for (size_t i = 1;; i += 2) {                                            \
        size_t datatype_index = i;                                             \
        size_t value_index    = i + 1;                                         \
        if (value_index >= length) break;                                      \
        (POINTER)->add(vec[datatype_index], vec[value_index]);                 \
      }                                                                        \
    }                                                                          \
}

    switch (type_) {
      // handle BRIDGE_REMAPTYPE_NONE as error. (see default)
      case BRIDGE_REMAPTYPE_KEYTOKEY:                       INITIALIZE_UNION_VALUE(p_.keyToKey,                       RemapFunc::KeyToKey);                       break;
      case BRIDGE_REMAPTYPE_KEYTOCONSUMER:                  INITIALIZE_UNION_VALUE(p_.keyToConsumer,                  RemapFunc::KeyToConsumer);                  break;
      case BRIDGE_REMAPTYPE_KEYTOPOINTINGBUTTON:            INITIALIZE_UNION_VALUE(p_.keyToPointingButton,            RemapFunc::KeyToPointingButton);            break;
      case BRIDGE_REMAPTYPE_CONSUMERTOCONSUMER:             INITIALIZE_UNION_VALUE(p_.consumerToConsumer,             RemapFunc::ConsumerToConsumer);             break;
      case BRIDGE_REMAPTYPE_CONSUMERTOKEY:                  INITIALIZE_UNION_VALUE(p_.consumerToKey,                  RemapFunc::ConsumerToKey);                  break;
      case BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER:            INITIALIZE_UNION_VALUE(p_.doublePressModifier,            RemapFunc::DoublePressModifier);            break;
      case BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP:              INITIALIZE_UNION_VALUE(p_.dropKeyAfterRemap,              RemapFunc::DropKeyAfterRemap);              break;
      case BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY:                INITIALIZE_UNION_VALUE(p_.holdingKeyToKey,                RemapFunc::HoldingKeyToKey);                break;
      case BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS:     INITIALIZE_UNION_VALUE(p_.ignoreMultipleSameKeyPress,     RemapFunc::IgnoreMultipleSameKeyPress);     break;
      case BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER:            INITIALIZE_UNION_VALUE(p_.keyOverlaidModifier,            RemapFunc::KeyOverlaidModifier);            break;
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOKEY:            INITIALIZE_UNION_VALUE(p_.pointingButtonToKey,            RemapFunc::PointingButtonToKey);            break;
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOPOINTINGBUTTON: INITIALIZE_UNION_VALUE(p_.pointingButtonToPointingButton, RemapFunc::PointingButtonToPointingButton); break;
      case BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL:       INITIALIZE_UNION_VALUE(p_.pointingRelativeToScroll,       RemapFunc::PointingRelativeToScroll);       break;
      case BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES:         INITIALIZE_UNION_VALUE(p_.simultaneousKeyPresses,         RemapFunc::SimultaneousKeyPresses);         break;
      default:
        IOLOG_ERROR("RemapClass::Item::initialize_remap unknown type_ (%d)\n", type_);
        type_ = BRIDGE_REMAPTYPE_NONE;
        goto error;
    }

#undef INITIALIZE_UNION_VALUE

    return;

  error:
    terminate();
  }

  void
  RemapClass::Item::initialize_filter(const unsigned int* vec, size_t length)
  {
    const unsigned int* end = NULL;
    unsigned int total = 0;

    // ------------------------------------------------------------
    // check parameters.
    //
    if (! vec || length <= 0) {
      IOLOG_ERROR("RemapClass::Item::initialize_filter invalid parameter.\n");
      goto error;
    }

    // ------------------------------------------------------------
    // initialize values.
    //
    end = vec + length;
    total = vec[0];
    vec += 1;

    if (total > 0) {
      filters_ = new RemapFilter::Vector_FilterUnionPointer();
      if (! filters_) {
        IOLOG_ERROR("RemapClass::Item::initialize_filter failed to allocate.\n");
        goto error;
      }

      for (unsigned int i = 0; i < total; ++i) {
        unsigned int size = 0;

        // (1) first check
        if (vec >= end) {
          IOLOG_ERROR("RemapClass::Item::initialize_filter invalid data.\n");
          goto error;
        }

        size = vec[0];
        vec += 1;

        if (vec >= end) {
          IOLOG_ERROR("RemapClass::Item::initialize_filter invalid data.\n");
          goto error;
        }

        RemapFilter::FilterUnion* newp = new RemapFilter::FilterUnion(vec, size);
        if (! newp) {
          IOLOG_ERROR("RemapClass::Item::initialize_filter failed to allocate.\n");
          goto error;
        }
        filters_->push_back(newp);
        vec += size;

        // Don't check vec position here.
        // Check vec position at (1) of next loop.
      }
    }

    return;

  error:
    terminate();
  }

  void
  RemapClass::Item::terminate(void)
  {
#define DELETE_UNLESS_NULL(POINTER) { \
    if (POINTER) { delete POINTER; }  \
}

    switch (type_) {
      case BRIDGE_REMAPTYPE_NONE:                                                                                  break;
      case BRIDGE_REMAPTYPE_KEYTOKEY:                       DELETE_UNLESS_NULL(p_.keyToKey);                       break;
      case BRIDGE_REMAPTYPE_KEYTOCONSUMER:                  DELETE_UNLESS_NULL(p_.keyToConsumer);                  break;
      case BRIDGE_REMAPTYPE_KEYTOPOINTINGBUTTON:            DELETE_UNLESS_NULL(p_.keyToPointingButton);            break;
      case BRIDGE_REMAPTYPE_CONSUMERTOCONSUMER:             DELETE_UNLESS_NULL(p_.consumerToConsumer);             break;
      case BRIDGE_REMAPTYPE_CONSUMERTOKEY:                  DELETE_UNLESS_NULL(p_.consumerToKey);                  break;
      case BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER:            DELETE_UNLESS_NULL(p_.doublePressModifier);            break;
      case BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP:              DELETE_UNLESS_NULL(p_.dropKeyAfterRemap);              break;
      case BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY:                DELETE_UNLESS_NULL(p_.holdingKeyToKey);                break;
      case BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS:     DELETE_UNLESS_NULL(p_.ignoreMultipleSameKeyPress);     break;
      case BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER:            DELETE_UNLESS_NULL(p_.keyOverlaidModifier);            break;
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOKEY:            DELETE_UNLESS_NULL(p_.pointingButtonToKey);            break;
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOPOINTINGBUTTON: DELETE_UNLESS_NULL(p_.pointingButtonToPointingButton); break;
      case BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL:       DELETE_UNLESS_NULL(p_.pointingRelativeToScroll);       break;
      case BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES:         DELETE_UNLESS_NULL(p_.simultaneousKeyPresses);         break;
      default:
        IOLOG_ERROR("RemapClass::Item::terminate unknown type_ (%d)\n", type_);
        break;
    }

#undef DELETE_UNLESS_NULL

    type_ = BRIDGE_REMAPTYPE_NONE;

    // ------------------------------------------------------------
    if (filters_) {
      for (size_t i = 0; i < filters_->size(); ++i) {
        RemapFilter::FilterUnion* p = (*filters_)[i];
        if (p) {
          delete p;
        }
      }
      delete filters_;
      filters_ = NULL;
    }
  }

  bool
  RemapClass::Item::remap(RemapParams& remapParams)
  {
    if (isblocked()) return false;

#define CALL_UNION_FUNCTION(POINTER) {                     \
    if (POINTER) { return (POINTER)->remap(remapParams); } \
}

    switch (type_) {
      case BRIDGE_REMAPTYPE_KEYTOKEY:                   CALL_UNION_FUNCTION(p_.keyToKey);                   break;
      case BRIDGE_REMAPTYPE_KEYTOCONSUMER:              CALL_UNION_FUNCTION(p_.keyToConsumer);              break;
      case BRIDGE_REMAPTYPE_KEYTOPOINTINGBUTTON:        CALL_UNION_FUNCTION(p_.keyToPointingButton);        break;
      case BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER:        CALL_UNION_FUNCTION(p_.doublePressModifier);        break;
      case BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY:            CALL_UNION_FUNCTION(p_.holdingKeyToKey);            break;
      case BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS: CALL_UNION_FUNCTION(p_.ignoreMultipleSameKeyPress); break;
      case BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER:        CALL_UNION_FUNCTION(p_.keyOverlaidModifier);        break;
      case BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES:     CALL_UNION_FUNCTION(p_.simultaneousKeyPresses);     break;
      default:
        // do nothing. (Do not call IOLOG_ERROR)
        break;
    }

#undef CALL_UNION_FUNCTION

    return false;
  }

  bool
  RemapClass::Item::remap(RemapConsumerParams& remapParams)
  {
    if (isblocked()) return false;

#define CALL_UNION_FUNCTION(POINTER) {                     \
    if (POINTER) { return (POINTER)->remap(remapParams); } \
}

    switch (type_) {
      case BRIDGE_REMAPTYPE_CONSUMERTOCONSUMER: CALL_UNION_FUNCTION(p_.consumerToConsumer); break;
      case BRIDGE_REMAPTYPE_CONSUMERTOKEY:      CALL_UNION_FUNCTION(p_.consumerToKey);      break;
      default:
        // do nothing. (Do not call IOLOG_ERROR)
        break;
    }

#undef CALL_UNION_FUNCTION

    return false;
  }

  bool
  RemapClass::Item::remap(RemapPointingParams_relative& remapParams)
  {
    if (isblocked()) return false;

#define CALL_UNION_FUNCTION(POINTER) {                     \
    if (POINTER) { return (POINTER)->remap(remapParams); } \
}

    switch (type_) {
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOKEY:            CALL_UNION_FUNCTION(p_.pointingButtonToKey);            break;
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOPOINTINGBUTTON: CALL_UNION_FUNCTION(p_.pointingButtonToPointingButton); break;
      case BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL:       CALL_UNION_FUNCTION(p_.pointingRelativeToScroll);       break;
      default:
        // do nothing. (Do not call IOLOG_ERROR)
        break;
    }

#undef CALL_UNION_FUNCTION

    return false;
  }

  bool
  RemapClass::Item::drop(const Params_KeyboardEventCallBack& params)
  {
    if (isblocked()) return false;

#define CALL_UNION_FUNCTION(POINTER) {               \
    if (POINTER) { return (POINTER)->drop(params); } \
}

    switch (type_) {
      case BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP: CALL_UNION_FUNCTION(p_.dropKeyAfterRemap); break;
      default:
        // do nothing. (Do not call IOLOG_ERROR)
        break;
    }

#undef CALL_UNION_FUNCTION

    return false;
  }

  void
  RemapClass::Item::remap_SimultaneousKeyPresses(void)
  {
    if (isblocked()) return;

#define CALL_UNION_FUNCTION(POINTER) {          \
    if (POINTER) { return (POINTER)->remap(); } \
}

    switch (type_) {
      case BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES: CALL_UNION_FUNCTION(p_.simultaneousKeyPresses); break;
      default:
        // do nothing. (Do not call IOLOG_ERROR)
        break;
    }

#undef CALL_UNION_FUNCTION
  }

  bool
  RemapClass::Item::isblocked(void)
  {
    if (! filters_) return false;

    for (size_t i = 0; i < filters_->size(); ++i) {
      RemapFilter::FilterUnion* p = (*filters_)[i];
      if (p && p->isblocked()) return true;
    }

    return false;
  }

  // ----------------------------------------------------------------------
  RemapClass::RemapClass(const unsigned int* vec, size_t length,
                         const unsigned int* filter, size_t filterlength,
                         unsigned int configindex, bool enable_when_passthrough) :
    configindex_(configindex), enable_when_passthrough_(enable_when_passthrough)
  {
  }

  RemapClass::~RemapClass(void)
  {
    for (size_t i = 0; i < items_.size(); ++i) {
      Item* p = items_[i];
      if (p) {
        p->terminate();
        delete p;
      }
    }
  }

  void
  RemapClass::remap_key(RemapParams& remapParams)
  {
    for (size_t i = 0; i < items_.size(); ++i) {
      Item* p = items_[i];
      if (p) {
        if (p->remap(remapParams)) return;
      }
    }
  }

  void
  RemapClass::remap_consumer(RemapConsumerParams& remapParams)
  {
    for (size_t i = 0; i < items_.size(); ++i) {
      Item* p = items_[i];
      if (p) {
        if (p->remap(remapParams)) return;
      }
    }
  }

  void
  RemapClass::remap_pointing(RemapPointingParams_relative& remapParams)
  {
    for (size_t i = 0; i < items_.size(); ++i) {
      Item* p = items_[i];
      if (p) {
        if (p->remap(remapParams)) return;
      }
    }
  }

  void
  RemapClass::remap_simultaneouskeypresses(void)
  {
    for (size_t i = 0; i < items_.size(); ++i) {
      Item* p = items_[i];
      if (p) {
        p->remap_SimultaneousKeyPresses();
      }
    }
  }

  bool
  RemapClass::remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params)
  {
    if (! enabled()) return false;

    for (size_t i = 0; i < items_.size(); ++i) {
      Item* p = items_[i];
      if (p) {
        if (p->drop(params)) return true;
      }
    }

    return false;
  }

  bool
  RemapClass::enabled(void)
  {
    if (config.notsave_passthrough) {
      // Pass-Through Mode
      if (! enable_when_passthrough_) {
        return false;
      }
    }

    // check configindex_ range
    if (configindex_ < 0 ||
        configindex_ >= sizeof(config.enabled_flags) / sizeof(config.enabled_flags[0])) {
      return false;
    }

    return config.enabled_flags[configindex_];
  }

  // ================================================================================
  namespace RemapClassManager {
    typedef void (*RemapClass_initialize)(void);
    typedef void (*RemapClass_terminate)(void);
    typedef void (*RemapClass_remap_setkeyboardtype)(KeyboardType& keyboardType);
    typedef void (*RemapClass_remap_key)(RemapParams& remapParams);
    typedef void (*RemapClass_remap_consumer)(RemapConsumerParams& remapParams);
    typedef void (*RemapClass_remap_pointing)(RemapPointingParams_relative& remapParams);
    typedef void (*RemapClass_remap_simultaneouskeypresses)(void);
    typedef bool (*RemapClass_remap_dropkeyafterremap)(const Params_KeyboardEventCallBack& params);
    typedef const char* (*RemapClass_get_statusmessage)(void);
    typedef bool (*RemapClass_enabled)(void);

#include "config/output/include.RemapClass.cpp"

    class Item : public List::Item {
    public:
      virtual ~Item(void) {}

      union {
        RemapClass_remap_setkeyboardtype remap_setkeyboardtype;
        RemapClass_remap_key remap_key;
        RemapClass_remap_consumer remap_consumer;
        RemapClass_remap_pointing remap_pointing;
        RemapClass_remap_simultaneouskeypresses remap_simultaneouskeypresses;
      } func;
    };

    List* queue_remap_setkeyboardtype_ = NULL;
    List* queue_remap_key_ = NULL;
    List* queue_remap_consumer_ = NULL;
    List* queue_remap_pointing_ = NULL;
    List* queue_remap_simultaneouskeypresses_ = NULL;
    IOLock* lock_ = NULL;
    TimerWrapper refresh_timer_;

    char statusmessage_[32];
    char lastmessage_[32];
    bool isEventInputQueueDelayEnabled_ = false;

    // ======================================================================
    static bool
    queue_isnotnull(void)
    {
      return queue_remap_setkeyboardtype_ &&
             queue_remap_key_ &&
             queue_remap_consumer_ &&
             queue_remap_pointing_ &&
             queue_remap_simultaneouskeypresses_;
    }

    static void
    cleanup_all(void)
    {
      if (queue_isnotnull()) {
        queue_remap_setkeyboardtype_->clear();
        queue_remap_key_->clear();
        queue_remap_consumer_->clear();
        queue_remap_pointing_->clear();
        queue_remap_simultaneouskeypresses_->clear();
      }
    }

    static void
    refresh_core(OSObject* owner, IOTimerEventSource* sender)
    {
      IOLockWrapper::ScopedLock lk(lock_);

      if (! queue_isnotnull()) return;

      KeyboardRepeat::cancel();

      cleanup_all();

      statusmessage_[0] = '\0';

      for (size_t i = 0;; ++i) {
        RemapClass_enabled enabled = listRemapClass_enabled[i];
        if (! enabled) break;
        if (! enabled()) continue;

#define PUSH_REMAPCLASS(ENTRY) {                        \
    if (listRemapClass_ ## ENTRY[i]) {                  \
      Item* newp = new Item();                          \
      (newp->func).ENTRY = listRemapClass_ ## ENTRY[i]; \
      queue_ ## ENTRY ## _->push_back(newp);            \
    }                                                   \
}

        PUSH_REMAPCLASS(remap_setkeyboardtype);
        PUSH_REMAPCLASS(remap_key);
        PUSH_REMAPCLASS(remap_consumer);
        PUSH_REMAPCLASS(remap_pointing);
        PUSH_REMAPCLASS(remap_simultaneouskeypresses);

#undef PUSH_REMAPCLASS

        if (listRemapClass_get_statusmessage[i]) {
          const char* msg = (listRemapClass_get_statusmessage[i])();
          if (msg) {
            strlcat(statusmessage_, msg, sizeof(statusmessage_));
            strlcat(statusmessage_, " ", sizeof(statusmessage_));
          }
        }
      }

      if (strcmp(statusmessage_, lastmessage_) != 0) {
        KeyRemap4MacBook_bridge::StatusMessage::Request request(KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_EXTRA, statusmessage_);
        KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE, &request, sizeof(request), NULL, 0);
        strlcpy(lastmessage_, statusmessage_, sizeof(lastmessage_));
      }

      if (queue_remap_simultaneouskeypresses_->empty()) {
        isEventInputQueueDelayEnabled_ = false;
      } else {
        isEventInputQueueDelayEnabled_ = true;
      }
    }

    // ======================================================================

    void
    initialize(IOWorkLoop& workloop)
    {
      lock_ = IOLockWrapper::alloc();
      statusmessage_[0] = '\0';
      lastmessage_[0] = '\0';

      for (size_t i = 0;; ++i) {
        RemapClass_initialize p = listRemapClass_initialize[i];
        if (! p) break;
        p();
      }

      queue_remap_setkeyboardtype_        = new List();
      queue_remap_key_                    = new List();
      queue_remap_consumer_               = new List();
      queue_remap_pointing_               = new List();
      queue_remap_simultaneouskeypresses_ = new List();

      refresh_timer_.initialize(&workloop, NULL, refresh_core);
    }

    void
    terminate(void)
    {
      refresh_timer_.terminate();

      if (queue_isnotnull()) {
        delete queue_remap_setkeyboardtype_;
        delete queue_remap_key_;
        delete queue_remap_consumer_;
        delete queue_remap_pointing_;
        delete queue_remap_simultaneouskeypresses_;
      }

      for (size_t i = 0;; ++i) {
        RemapClass_terminate p = listRemapClass_terminate[i];
        if (! p) break;
        p();
      }

      IOLockWrapper::free(lock_);
    }

    void
    refresh(void)
    {
      // We use timer to prevent deadlock of lock_. (refresh may be called in the remap_key, remap_consumer, *).
      enum {
        DELAY = 1,
      };
      refresh_timer_.setTimeoutMS(DELAY);
    }

    // ----------------------------------------------------------------------
#define DECLARE_REMAPFUNC(QUEUE, FUNC, PARAMS)    \
  {                                               \
    IOLockWrapper::ScopedLock lk(lock_);          \
                                                  \
    if (! QUEUE) return;                          \
                                                  \
    Item* p = static_cast<Item*>(QUEUE->front()); \
    for (;;) {                                    \
      if (! p) break;                             \
      if (! (p->func).FUNC) break;                \
      (p->func).FUNC(PARAMS);                     \
      p = static_cast<Item*>(p->getnext());       \
    }                                             \
  }

    void
    remap_setkeyboardtype(KeyboardType& keyboardType)
    {
      DECLARE_REMAPFUNC(queue_remap_setkeyboardtype_, remap_setkeyboardtype, keyboardType);
    }

    void
    remap_key(RemapParams& remapParams)
    {
      DECLARE_REMAPFUNC(queue_remap_key_, remap_key, remapParams);
    }

    void
    remap_consumer(RemapConsumerParams& remapParams)
    {
      DECLARE_REMAPFUNC(queue_remap_consumer_, remap_consumer, remapParams);
    }

    void
    remap_pointing(RemapPointingParams_relative& remapParams)
    {
      DECLARE_REMAPFUNC(queue_remap_pointing_, remap_pointing, remapParams);
    }

    void
    remap_simultaneouskeypresses(void)
    {
      DECLARE_REMAPFUNC(queue_remap_simultaneouskeypresses_, remap_simultaneouskeypresses, );
    }

    bool
    remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params)
    {
      // We do not need a lock, because we don't refer queues.

      bool dropped = false;
      for (size_t i = 0;; ++i) {
        RemapClass_remap_dropkeyafterremap p = listRemapClass_remap_dropkeyafterremap[i];
        if (! p) break;
        if (p(params)) dropped = true;
      }
      return dropped;
    }

    bool
    isEventInputQueueDelayEnabled(void)
    {
      return isEventInputQueueDelayEnabled_;
    }
  }
}
