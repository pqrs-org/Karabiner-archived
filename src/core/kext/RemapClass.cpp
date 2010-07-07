#include "RemapClass.hpp"
#include "Client.hpp"
#include "KeyboardRepeat.hpp"
#include "util/CommonData.hpp"
#include "util/EventInputQueue.hpp"
#include "RemapFunc/KeyToKey.hpp"
#include "RemapFunc/KeyToConsumer.hpp"
#include "RemapFunc/KeyToPointingButton.hpp"
#include "RemapFunc/ConsumerToConsumer.hpp"
#include "RemapFunc/ConsumerToKey.hpp"
#include "RemapFunc/DoublePressModifier.hpp"
#include "RemapFunc/DropKeyAfterRemap.hpp"
#include "RemapFunc/HoldingKeyToKey.hpp"
#include "RemapFunc/IgnoreMultipleSameKeyPress.hpp"
#include "RemapFunc/KeyOverlaidModifier.hpp"
#include "RemapFunc/PointingButtonToKey.hpp"
#include "RemapFunc/PointingButtonToPointingButton.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"
#include "RemapFunc/SimultaneousKeyPresses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapClassManager {
    typedef void (*RemapClass_initialize)(void);
    typedef void (*RemapClass_terminate)(void);
    typedef bool (*RemapClass_handlevirtualkey)(const Params_KeyboardEventCallBack& params);
    typedef void (*RemapClass_remap_setkeyboardtype)(KeyboardType& keyboardType);
    typedef void (*RemapClass_remap_key)(RemapParams& remapParams);
    typedef void (*RemapClass_remap_consumer)(RemapConsumerParams& remapParams);
    typedef void (*RemapClass_remap_pointing)(RemapPointingParams_relative& remapParams);
    typedef void (*RemapClass_remap_simultaneouskeypresses)(void);
    typedef bool (*RemapClass_remap_dropkeyafterremap)(const Params_KeyboardEventCallBack& params);
    typedef const char* (*RemapClass_get_statusmessage)(void);
    typedef bool (*RemapClass_enabled)(void);

#include "config/output/include.RemapClass.cpp"

    class Item : public Queue::Item {
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

    Queue* queue_remap_setkeyboardtype_ = NULL;
    Queue* queue_remap_key_ = NULL;
    Queue* queue_remap_consumer_ = NULL;
    Queue* queue_remap_pointing_ = NULL;
    Queue* queue_remap_simultaneouskeypresses_ = NULL;
    IOLock* lock_ = NULL;
    TimerWrapper refresh_timer_;

    char statusmessage_[32];
    char lastmessage_[32];
    bool isEventInputQueueDelayEnabled_ = false;

    // ======================================================================
    static void
    cleanup(Queue* q)
    {
      for (;;) {
        Item* p = static_cast<Item*>(q->front());
        if (! p) break;

        q->pop();
        delete p;
      }
    }

    static void
    cleanup_all(void)
    {
      cleanup(queue_remap_setkeyboardtype_);
      cleanup(queue_remap_key_);
      cleanup(queue_remap_consumer_);
      cleanup(queue_remap_pointing_);
      cleanup(queue_remap_simultaneouskeypresses_);
    }

    static void
    refresh_core(OSObject* owner, IOTimerEventSource* sender)
    {
      IOLockWrapper::ScopedLock lk(lock_);

      KeyboardRepeat::cancel();

      cleanup_all();

      statusmessage_[0] = '\0';

      for (size_t i = 0;; ++i) {
        RemapClass_enabled enabled = listRemapClass_enabled[i];
        if (! enabled) break;
        if (! enabled()) continue;

#define PUSH_REMAPCLASS(ENTRY) {                               \
    if (listRemapClass_ ## ENTRY[i] && queue_ ## ENTRY ## _) { \
      Item* newp = new Item();                                 \
      (newp->func).ENTRY = listRemapClass_ ## ENTRY[i];        \
      queue_ ## ENTRY ## _->push(newp);                        \
    }                                                          \
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

      if (queue_remap_simultaneouskeypresses_ && ! queue_remap_simultaneouskeypresses_->empty()) {
        isEventInputQueueDelayEnabled_ = true;
      } else {
        isEventInputQueueDelayEnabled_ = false;
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

      queue_remap_setkeyboardtype_ = new Queue();
      queue_remap_key_ = new Queue();
      queue_remap_consumer_ = new Queue();
      queue_remap_pointing_ = new Queue();
      queue_remap_simultaneouskeypresses_ = new Queue();

      refresh_timer_.initialize(&workloop, NULL, refresh_core);
    }

    void
    terminate(void)
    {
      cleanup_all();

      refresh_timer_.terminate();

      delete queue_remap_setkeyboardtype_;
      delete queue_remap_key_;
      delete queue_remap_consumer_;
      delete queue_remap_pointing_;
      delete queue_remap_simultaneouskeypresses_;

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
    handlevirtualkey(const Params_KeyboardEventCallBack& params)
    {
      // We do not need a lock, because we don't refer queues.

      for (size_t i = 0;; ++i) {
        RemapClass_handlevirtualkey p = listRemapClass_handlevirtualkey[i];
        if (! p) break;
        if (p(params)) return true;
      }
      return false;
    }

    bool
    isEventInputQueueDelayEnabled(void)
    {
      return isEventInputQueueDelayEnabled_;
    }
  }
}
