#ifndef WEAKPOINTER_HPP
#define WEAKPOINTER_HPP

#include "List.hpp"

namespace org_pqrs_Karabiner {

#define DECLARE_WEAKPOINTER(TYPENAME)                                                            \
  class TYPENAME;                                                                                \
  class WeakPointerManager_##TYPENAME final {                                                    \
  public:                                                                                        \
    /* Call WeakPointerManager::add in constructor. */                                           \
    /* For example: */                                                                           \
                                                                                                 \
    /* ----------------------------------------------- */                                        \
    /* DECLARE_WEAKPOINTER(TestClass);                 */                                        \
    /*                                                 */                                        \
    /* class TestClass final {                         */                                        \
    /* public:                                         */                                        \
    /*   TestClass(void) {                             */                                        \
    /*     WeakPointerManager_TestClass::add(this);    */                                        \
    /*   }                                             */                                        \
    /*   ~TestClass(void) {                            */                                        \
    /*     WeakPointerManager_TestClass::remove(this); */                                        \
    /*   }                                             */                                        \
    /* };                                              */                                        \
    /* ----------------------------------------------- */                                        \
                                                                                                 \
    /* Note: */                                                                                  \
    /* DO NOT call WeakPointerManager::add twice with same pointer. */                           \
    /* If you call WeakPointerManager::add twice, */                                             \
    /* WeakPointerManager::expired will return false with deleted pointer. */                    \
                                                                                                 \
    static void add(const TYPENAME* p) {                                                         \
      auto item = new WeakPointerManagerItem(p, ++lastid_);                                      \
      if (item) {                                                                                \
        list_.push_back(item);                                                                   \
      }                                                                                          \
    }                                                                                            \
                                                                                                 \
    static void remove(const TYPENAME* pointer) {                                                \
      for (WeakPointerManagerItem* p = static_cast<WeakPointerManagerItem*>(list_.safe_front()); \
           p;                                                                                    \
           p = static_cast<WeakPointerManagerItem*>(p->getnext())) {                             \
        if (p->pointer == pointer) {                                                             \
          list_.erase_and_delete(p);                                                             \
          break;                                                                                 \
        }                                                                                        \
      }                                                                                          \
    }                                                                                            \
                                                                                                 \
    static bool expired(const TYPENAME* pointer, int id) {                                       \
      for (WeakPointerManagerItem* p = static_cast<WeakPointerManagerItem*>(list_.safe_front()); \
           p;                                                                                    \
           p = static_cast<WeakPointerManagerItem*>(p->getnext())) {                             \
        if (p->pointer == pointer && p->id == id) {                                              \
          return false;                                                                          \
        }                                                                                        \
      }                                                                                          \
      return true;                                                                               \
    }                                                                                            \
                                                                                                 \
    static int getid(const TYPENAME* pointer) {                                                  \
      for (WeakPointerManagerItem* p = static_cast<WeakPointerManagerItem*>(list_.safe_front()); \
           p;                                                                                    \
           p = static_cast<WeakPointerManagerItem*>(p->getnext())) {                             \
        if (p->pointer == pointer) {                                                             \
          return p->id;                                                                          \
        }                                                                                        \
      }                                                                                          \
      return -1;                                                                                 \
    }                                                                                            \
                                                                                                 \
  private:                                                                                       \
    class WeakPointerManagerItem final : public List::Item {                                     \
    public:                                                                                      \
      WeakPointerManagerItem(const TYPENAME* p, int c) : pointer(p), id(c) {}                    \
      ~WeakPointerManagerItem(void) {}                                                           \
                                                                                                 \
      const TYPENAME* pointer;                                                                   \
      int id;                                                                                    \
    };                                                                                           \
                                                                                                 \
    static List list_;                                                                           \
    static int lastid_;                                                                          \
  };                                                                                             \
                                                                                                 \
  class WeakPointer_##TYPENAME final {                                                           \
  public:                                                                                        \
    WeakPointer_##TYPENAME(TYPENAME* p) : pointer_(p),                                           \
                                          id_(WeakPointerManager_##TYPENAME::getid(p)) {}        \
    bool expired(void) const { return WeakPointerManager_##TYPENAME::expired(pointer_, id_); }   \
                                                                                                 \
    TYPENAME* operator->(void) const { return pointer_; }                                        \
    TYPENAME* get(void) const { return pointer_; }                                               \
                                                                                                 \
    bool operator==(WeakPointer_##TYPENAME other) const {                                        \
      return pointer_ == other.pointer_ && id_ == other.id_;                                     \
    }                                                                                            \
                                                                                                 \
  private:                                                                                       \
    TYPENAME* pointer_;                                                                          \
    int id_;                                                                                     \
  };

#define DEFINE_WEAKPOINTER(TYPENAME)         \
  List WeakPointerManager_##TYPENAME::list_; \
  int WeakPointerManager_##TYPENAME::lastid_ = 0;

#define DEFINE_WEAKPOINTER_IN_CLASS(CLASS, TYPENAME) \
  List CLASS::WeakPointerManager_##TYPENAME::list_;  \
  int CLASS::WeakPointerManager_##TYPENAME::lastid_ = 0;
}

#endif
