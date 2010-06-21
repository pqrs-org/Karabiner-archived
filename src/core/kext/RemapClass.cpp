#include "RemapClass.hpp"
#include "Client.hpp"
#include "KeyboardRepeat.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapClassManager {
    // DUMMY CODE
    RemapClass* listRemapClass[] = { NULL };

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

    // ======================================================================
    void
    initialize(void)
    {
      lock_ = IOLockWrapper::alloc();
      statusmessage_[0] = '\0';
      lastmessage_[0] = '\0';
    }

    void
    terminate(void)
    {
      cleanup_all();
      IOLockWrapper::free(lock_);
    }

    void
    refresh(void)
    {
      IOLockWrapper::ScopedLock lk(lock_);

      KeyboardRepeat::cancel();

      cleanup_all();

      statusmessage_[0] = '\0';

      for (unsigned int i = 0;; ++i) {
        RemapClass* p = listRemapClass[i];
        if (! listRemapClass[i]) break;

        if (p->enabled(RemapClass::ENABLE_TYPE_KEY)) queue_remap_key_->push(new Item(p));
        if (p->enabled(RemapClass::ENABLE_TYPE_CONSUMER)) queue_remap_consumer_->push(new Item(p));
        if (p->enabled(RemapClass::ENABLE_TYPE_POINTING)) queue_remap_pointing_->push(new Item(p));
        if (p->enabled(RemapClass::ENABLE_TYPE_SETKEYBOARDTYPE)) queue_remap_setkeyboardtype_->push(new Item(p));

        if (p->enabled(RemapClass::ENABLE_TYPE_STATUSMESSAGE)) {
          strlcat(statusmessage_, p->statusmessage, sizeof(statusmessage_));
          strlcat(statusmessage_, " ", sizeof(statusmessage_));
        }
      }

      if (strcmp(statusmessage_, lastmessage_) != 0) {
        KeyRemap4MacBook_bridge::StatusMessage::Request request(KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_EXTRA, statusmessage_);
        KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE, &request, sizeof(request), NULL, 0);
      }
      strlcpy(lastmessage_, statusmessage_, sizeof(lastmessage_));
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
