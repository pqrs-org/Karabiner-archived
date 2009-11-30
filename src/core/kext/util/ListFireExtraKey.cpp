#include "listFireExtraKey.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace ListFireExtraKey {
    enum {
      MAXNUM = 32,
    };

    class Item {
    public:
      void set(unsigned int _eventType, unsigned int _flags, unsigned int _key) {
        eventType = _eventType;
        flags = _flags;
        key = _key;
      }
      unsigned int getEventType(void) const { return eventType; }
      unsigned int getFlags(void) const { return flags; }
      unsigned int getKey(void) const { return key; }

    private:
      unsigned int eventType;
      unsigned int flags;
      unsigned int key;
    };

    Item item[MAXNUM];
    int size;

    void
    reset(void)
    {
      size = 0;
    }

    bool
    isEmpty(void)
    {
      return size == 0;
    }

    void
    add(unsigned int eventType, unsigned int flags, unsigned int key)
    {
      if (size >= MAXNUM) return;
      item[size].set(eventType, flags, key);
      ++size;
    }

    void
    addKey(unsigned int flags, KeyCode::KeyCode keyCode)
    {
      if (RemapUtil::getKeyCodeModifier(keyCode) != ModifierFlag::NONE) {
        add(KeyEvent::MODIFY, flags, keyCode);
      } else {
        add(KeyEvent::DOWN, flags, keyCode);
        add(KeyEvent::UP, flags, keyCode);
      }
    }

    void
    fire(KeyboardEventCallback callback, const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata)
    {
      if (callback == NULL) return;

      Params_KeyboardEventCallBack callbackparams = params;

      for (int i = 0; i < size; ++i) {
        callbackparams.eventType = item[i].getEventType();
        callbackparams.flags = item[i].getFlags();
        callbackparams.key = item[i].getKey();

        RemapUtil::fireKey(callback, callbackparams, workspacedata);
      }
    }
  }
}
