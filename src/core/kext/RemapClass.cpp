#include "bridge.h"
#include "RemapClass.hpp"
#include "Client.hpp"
#include "KeyboardRepeat.hpp"
#include "util/CommonData.hpp"
#include "util/EventInputQueue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  RemapClass::Item::initialize(unsigned int* vec)
  {
    type_ = BRIDGE_REMAPTYPE_NONE;
    if (! vec) return;

    // ------------------------------------------------------------
    // handle type_.
    type_ = vec[0];
    switch (type_) {
      // handle BRIDGE_REMAPTYPE_NONE as error. (see default)
      case BRIDGE_REMAPTYPE_KEYTOKEY:                       p_.keyToKey                       = new RemapFunc::KeyToKey;                       break;
      case BRIDGE_REMAPTYPE_KEYTOCONSUMER:                  p_.keyToConsumer                  = new RemapFunc::KeyToConsumer;                  break;
      case BRIDGE_REMAPTYPE_KEYTOPOINTINGBUTTON:            p_.keyToPointingButton            = new RemapFunc::KeyToPointingButton;            break;
      case BRIDGE_REMAPTYPE_CONSUMERTOCONSUMER:             p_.consumerToConsumer             = new RemapFunc::ConsumerToConsumer;             break;
      case BRIDGE_REMAPTYPE_CONSUMERTOKEY:                  p_.consumerToKey                  = new RemapFunc::ConsumerToKey;                  break;
      case BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER:            p_.doublePressModifier            = new RemapFunc::DoublePressModifier;            break;
      case BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP:              p_.dropKeyAfterRemap              = new RemapFunc::DropKeyAfterRemap;              break;
      case BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY:                p_.holdingKeyToKey                = new RemapFunc::HoldingKeyToKey;                break;
      case BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS:     p_.ignoreMultipleSameKeyPress     = new RemapFunc::IgnoreMultipleSameKeyPress;     break;
      case BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER:            p_.keyOverlaidModifier            = new RemapFunc::KeyOverlaidModifier;            break;
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOKEY:            p_.pointingButtonToKey            = new RemapFunc::PointingButtonToKey;            break;
      case BRIDGE_REMAPTYPE_POINTINGBUTTONTOPOINTINGBUTTON: p_.pointingButtonToPointingButton = new RemapFunc::PointingButtonToPointingButton; break;
      case BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL:       p_.pointingRelativeToScroll       = new RemapFunc::PointingRelativeToScroll;       break;
      case BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES:         p_.simultaneousKeyPresses         = new RemapFunc::SimultaneousKeyPresses;         break;
      default:
        IOLOG_ERROR("RemapClass::Item::initialize unknown type_ (%d)\n", type_);
        break;
    }
  }

  void
  RemapClass::Item::terminate(void)
  {
#define DELETE_UNLESS_NULL(POINTER) { \
    if (POINTER) {                    \
      delete POINTER;                 \
    }                                 \
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
  }

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
