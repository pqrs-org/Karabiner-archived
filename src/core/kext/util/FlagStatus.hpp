#ifndef FLAGSTATUS_HPP
#define FLAGSTATUS_HPP

#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class FlagStatus {
  public:
    class Item {
      friend class FlagStatus;

    public:
      Item(void) : flag_(ModifierFlag::NONE) {}

    private:
      void initialize(ModifierFlag f);
      void set(void);
      void set(KeyCode key, Flags flags);

      void reset(void);
      Flags makeFlag(void) const {
        if (count_ + temporary_count_ + lock_count_ + original_lock_count_ > 0) {
          return flag_;
        } else {
          return 0;
        }
      }

      void increase(void);
      void decrease(void);
      void temporary_increase(void) { ++temporary_count_; }
      void temporary_decrease(void) { --temporary_count_; }
      void lock_increase(void) { lock_count_ = 1; }
      void lock_decrease(void) { lock_count_ = 0; }
      void lock_toggle(void)   { lock_count_ = ! lock_count_; }

      ModifierFlag flag_;
      int count_;
      int temporary_count_;

      int lock_count_; // store remapped lock status. (CapsLock, FN lock, ...)
      int original_lock_count_; // store original CapsLock status.
    };
    enum { MAXNUM = 16 };

    static bool initialize(void);
    static void set(void);
    static void set(KeyCode key, Flags flags);
    static Flags makeFlags(void);
    // get registered ModifierFlag by index.
    static ModifierFlag getFlag(int index);
    static void reset(void);

    static Flags getLockedFlags(void);

    static void increase(Flags flags);
    static void decrease(Flags flags);
    static void temporary_increase(Flags flags);
    static void temporary_decrease(Flags flags);
    static void lock_increase(Flags flags);
    static void lock_decrease(Flags flags);
    static void lock_toggle(Flags flags);

  private:
    static Item item_[MAXNUM];
  };
}

#endif
