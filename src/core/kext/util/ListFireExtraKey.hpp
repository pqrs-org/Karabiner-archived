#ifndef LISTFIREEXTRAKEY_HPP
#define LISTFIREEXTRAKEY_HPP

#include "base.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListFireExtraKey {
  public:
    enum {
      FIREEXTRAKEY_MAXNUM = 32,
    };
    void reset(void) { size = 0; }
    bool isEmpty(void) { return size == 0; }
    void add(unsigned int eventType, unsigned int flags, unsigned int key, unsigned int charCode) {
      if (size >= FIREEXTRAKEY_MAXNUM) return;
      list[size].set(eventType, flags, key, charCode);
      ++size;
    }
    void fire(KeyboardEventCallback callback,
              OSObject *target,
              unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet, unsigned int keyboardType,
              AbsoluteTime ts, OSObject *sender, void *refcon);

    // utility
    void addKey(unsigned int flags, KeyCode::KeyCode keyCode, CharCode::CharCode charCode);

  private:
    class Item {
    public:
      void set(unsigned int _eventType, unsigned int _flags, unsigned int _key, unsigned int _charCode) {
        eventType = _eventType;
        flags = _flags;
        key = _key;
        charCode = _charCode;
      }
      unsigned int getEventType(void) const { return eventType; }
      unsigned int getFlags(void) const { return flags; }
      unsigned int getKey(void) const { return key; }
      unsigned int getCharCode(void) const { return charCode; }

    private:
      unsigned int eventType;
      unsigned int flags;
      unsigned int key;
      unsigned int charCode;
    };

    Item list[FIREEXTRAKEY_MAXNUM];
    int size;
  };
}

#endif
