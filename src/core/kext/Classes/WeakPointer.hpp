#ifndef WEAKPOINTER_HPP
#define WEAKPOINTER_HPP

#include "List.hpp"

namespace org_pqrs_Karabiner {

#define DECLARE_WEAKPOINTER(TYPENAME)                                                                   \
  class TYPENAME;                                                                                       \
  class WeakPointerManager_##TYPENAME final {                                                           \
  public:                                                                                               \
    /* Call WeakPointerManager::add in constructor. */                                                  \
    /* For example: */                                                                                  \
                                                                                                        \
    /* ----------------------------------------------- */                                               \
    /* DECLARE_WEAKPOINTER(TestClass);                 */                                               \
    /*                                                 */                                               \
    /* class TestClass final {                         */                                               \
    /* public:                                         */                                               \
    /*   TestClass(void) {                             */                                               \
    /*     WeakPointerManager_TestClass::add(this);    */                                               \
    /*   }                                             */                                               \
    /*   ~TestClass(void) {                            */                                               \
    /*     WeakPointerManager_TestClass::remove(this); */                                               \
    /*   }                                             */                                               \
    /* };                                              */                                               \
    /* ----------------------------------------------- */                                               \
                                                                                                        \
    /* Note: */                                                                                         \
    /* DO NOT call WeakPointerManager::add twice with same pointer. */                                  \
    /* If you call WeakPointerManager::add twice, */                                                    \
    /* WeakPointerManager::expired will return false with deleted pointer. */                           \
                                                                                                        \
    static void add(const TYPENAME* p) {                                                                \
      auto item = new Item(p);                                                                          \
      if (item) {                                                                                       \
        list_.push_back(item);                                                                          \
      }                                                                                                 \
    }                                                                                                   \
                                                                                                        \
    static void remove(const TYPENAME* pointer) {                                                       \
      for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) { \
        if (p->pointer == pointer) {                                                                    \
          list_.erase_and_delete(p);                                                                    \
          break;                                                                                        \
        }                                                                                               \
      }                                                                                                 \
    }                                                                                                   \
                                                                                                        \
    static bool expired(const TYPENAME* pointer) {                                                      \
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
      Item(const TYPENAME* p) : pointer(p) {}                                                           \
      virtual ~Item(void) {}                                                                            \
                                                                                                        \
      const TYPENAME* pointer;                                                                          \
    };                                                                                                  \
                                                                                                        \
    static List list_;                                                                                  \
  };                                                                                                    \
                                                                                                        \
  class WeakPointer_##TYPENAME final {                                                                  \
  public:                                                                                               \
    WeakPointer_##TYPENAME(const TYPENAME* p) : pointer_(p) {}                                          \
    bool expired(void) const { return WeakPointerManager_##TYPENAME::expired(pointer_); }               \
                                                                                                        \
  private:                                                                                              \
    const TYPENAME* pointer_;                                                                           \
  };

#define DEFINE_WEAKPOINTER(TYPENAME) \
  List WeakPointerManager_##TYPENAME::list_;
}

#endif
