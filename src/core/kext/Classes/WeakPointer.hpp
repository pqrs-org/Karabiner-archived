#ifndef WEAKPOINTER_HPP
#define WEAKPOINTER_HPP

#include "List.hpp"

namespace org_pqrs_Karabiner {

#define DECLARE_WEAKPOINTER(TYPENAME)                                                                   \
  class TYPENAME;                                                                                       \
  class WeakPointerManager_##TYPENAME final {                                                           \
  public:                                                                                               \
    static void add(TYPENAME* p) {                                                                      \
      auto item = new Item(p);                                                                          \
      if (item) {                                                                                       \
        list_.push_back(item);                                                                          \
      }                                                                                                 \
    }                                                                                                   \
                                                                                                        \
    static void remove(TYPENAME* pointer) {                                                             \
      Item* p = static_cast<Item*>(list_.safe_front());                                                 \
      for (;;) {                                                                                        \
        if (!p) break;                                                                                  \
        if (p->pointer == pointer) {                                                                    \
          p = static_cast<Item*>(list_.erase_and_delete(p));                                            \
        } else {                                                                                        \
          p = static_cast<Item*>(p->getnext());                                                         \
        }                                                                                               \
      }                                                                                                 \
    }                                                                                                   \
                                                                                                        \
    static bool expired(TYPENAME* pointer) {                                                            \
      for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) { \
        if (p->pointer == pointer) {                                                                    \
          return false;                                                                                 \
        }                                                                                               \
      }                                                                                                 \
      return true;                                                                                      \
    }                                                                                                   \
                                                                                                        \
  private:                                                                                              \
    class Item : public List::Item {                                                                    \
    public:                                                                                             \
      Item(TYPENAME* p) : pointer(p) {}                                                                 \
      virtual ~Item(void) {}                                                                            \
                                                                                                        \
      TYPENAME* pointer;                                                                                \
    };                                                                                                  \
                                                                                                        \
    static List list_;                                                                                  \
  };                                                                                                    \
                                                                                                        \
  class WeakPointer_##TYPENAME final {                                                                  \
  public:                                                                                               \
    WeakPointer_##TYPENAME(TYPENAME* p) : pointer_(p) {}                                                \
    bool expired(void) const { return WeakPointerManager_##TYPENAME::expired(pointer_); }               \
                                                                                                        \
  private:                                                                                              \
    TYPENAME* pointer_;                                                                                 \
  };

#define DEFINE_WEAKPOINTER(TYPENAME) \
  List WeakPointerManager_##TYPENAME::list_;
}

#endif
