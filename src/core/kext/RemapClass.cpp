#include "RemapClass.hpp"
#include "Client.hpp"
#include "KeyboardRepeat.hpp"
#include "RemapUtil.hpp"
#include "util/CommonData.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapClassManager {
#include "config/output/include.RemapClass.cpp"

    class Item : public Queue::Item {
    public:
      Item(void) : remapclass(NULL) {}
      Item(RemapClass* rc) : remapclass(rc) {}
      virtual ~Item(void) {}

      RemapClass* remapclass;
    };
    Queue* queue_remap_setkeyboardtype_ = NULL;
    Queue* queue_remap_key_ = NULL;
    Queue* queue_remap_consumer_ = NULL;
    Queue* queue_remap_pointing_ = NULL;
    IOLock* lock_ = NULL;
    TimerWrapper refresh_timer_;

    char statusmessage_[32];
    char lastmessage_[32];

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
    }

    static void
    refresh_core(OSObject* owner, IOTimerEventSource* sender)
    {
      IOLockWrapper::ScopedLock lk(lock_);

      KeyboardRepeat::cancel();

      cleanup_all();

      statusmessage_[0] = '\0';

      for (size_t i = 0;; ++i) {
        RemapClass* p = listRemapClass[i];
        if (! listRemapClass[i]) break;

        if (p->enabled(RemapClass::ENABLE_TYPE_SETKEYBOARDTYPE) && queue_remap_setkeyboardtype_) queue_remap_setkeyboardtype_->push(new Item(p));
        if (p->enabled(RemapClass::ENABLE_TYPE_KEY)             && queue_remap_key_)             queue_remap_key_->push(new Item(p));
        if (p->enabled(RemapClass::ENABLE_TYPE_CONSUMER)        && queue_remap_consumer_)        queue_remap_consumer_->push(new Item(p));
        if (p->enabled(RemapClass::ENABLE_TYPE_POINTING)        && queue_remap_pointing_)        queue_remap_pointing_->push(new Item(p));

        if (p->enabled(RemapClass::ENABLE_TYPE_STATUSMESSAGE)) {
          if (p->statusmessage) {
            strlcat(statusmessage_, p->statusmessage, sizeof(statusmessage_));
            strlcat(statusmessage_, " ", sizeof(statusmessage_));
          }
        }
      }

      if (strcmp(statusmessage_, lastmessage_) != 0) {
        KeyRemap4MacBook_bridge::StatusMessage::Request request(KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_EXTRA, statusmessage_);
        KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE, &request, sizeof(request), NULL, 0);
        strlcpy(lastmessage_, statusmessage_, sizeof(lastmessage_));
      }
    }

    // ======================================================================

    void
    initialize(IOWorkLoop& workloop)
    {
      lock_ = IOLockWrapper::alloc();
      statusmessage_[0] = '\0';
      lastmessage_[0] = '\0';

      queue_remap_setkeyboardtype_ = new Queue();
      queue_remap_key_ = new Queue();
      queue_remap_consumer_ = new Queue();
      queue_remap_pointing_ = new Queue();

      for (size_t i = 0;; ++i) {
        RemapClass* p = listRemapClass[i];
        if (! listRemapClass[i]) break;
        p->initialize();
      }

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
      if (! p->remapclass) break;                 \
      p->remapclass->FUNC(PARAMS);                \
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
  }
}
