#ifndef LISTFIREEXTRAKEY_HPP
#define LISTFIREEXTRAKEY_HPP

#include "base.hpp"
#include "keycode.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListFireExtraKey {
  public:
    enum {
      FIREEXTRAKEY_MAXNUM = 32,
    };
    void reset(void) { size = 0; }
    bool isEmpty(void) { return size == 0; }
    void add(unsigned int eventType, unsigned int flags, unsigned int key) {
      if (size >= FIREEXTRAKEY_MAXNUM) return;
      list[size].set(eventType, flags, key);
      ++size;
    }
    void fire(KeyboardEventCallback callback, const Params_KeyboardEventCallBack &params);

    // utility
    void addKey(unsigned int flags, KeyCode::KeyCode keyCode);

  private:
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

    Item list[FIREEXTRAKEY_MAXNUM];
    int size;
  };
}

#endif
