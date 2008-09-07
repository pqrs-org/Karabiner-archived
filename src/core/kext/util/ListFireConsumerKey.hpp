#ifndef LISTFIRECONSUMERKEY_HPP
#define LISTFIRECONSUMERKEY_HPP

#include "base.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListFireConsumerKey {
  public:
    enum {
      FIRECONSUMERKEY_MAXNUM = 4,
    };
    void reset(void) { size = 0; }
    bool isEmpty(void) { return size == 0; }
    void add(unsigned int eventType, unsigned int flags, unsigned int key) {
      if (size >= FIRECONSUMERKEY_MAXNUM) return;
      list[size].set(eventType, flags, key);
      ++size;
    }
    void fire(KeyboardSpecialEventCallback callback,
              OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon);

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
    Item list[FIRECONSUMERKEY_MAXNUM];
    int size;
  };
}

#endif
