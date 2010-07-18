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
        if (count_ + temporary_count_ + lock_count_ + sticky_count_ > 0) {
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
      void sticky_increase(void) { sticky_count_ = 1; }
      void sticky_decrease(void) { sticky_count_ = 0; }
      void sticky_toggle(void) { sticky_count_ = ! sticky_count_; }

      ModifierFlag flag_;
      int count_;
      int temporary_count_;

      int lock_count_; // store remapped lock status. (CapsLock, FN lock, ...)

      int sticky_count_;
    };

    class ScopedTemporaryFlagsChanger {
    public:
      ScopedTemporaryFlagsChanger(Flags toFlags) {
        count_ = new int[MAXNUM];
        if (! count_) return;

        for (int i = 0;; ++i) {
          count_[i] = 0;

          ModifierFlag flag = getFlag(i);
          if (flag == ModifierFlag::NONE) break;

          // reset flag
          while (makeFlags().isOn(flag)) {
            temporary_decrease(flag);
            --count_[i];
          }

          // set a flag
          if (toFlags.isOn(flag)) {
            temporary_increase(flag);
            ++count_[i];
          }
        }
      }
      ~ScopedTemporaryFlagsChanger(void) {
        if (! count_) return;

        for (int i = 0;; ++i) {
          ModifierFlag flag = getFlag(i);
          if (flag == ModifierFlag::NONE) break;

          if (count_[i] < 0) {
            while (count_[i] != 0) {
              temporary_increase(flag);
              ++count_[i];
            }
          } else if (count_[i] > 0) {
            while (count_[i] != 0) {
              temporary_decrease(flag);
              --count_[i];
            }
          }
        }

        delete[] count_;
      }

    private:
      int* count_;
    };

    enum { MAXNUM = 32 };

    static bool initialize(void);
    static void set(void);
    static void set(KeyCode key, Flags flags);
    static Flags makeFlags(void);
    // get registered ModifierFlag by index.
    static ModifierFlag getFlag(int index);
    static void reset(void);

    // getLockedFlags returns only Virtual locks (not hardware CapsLock).
    static Flags getLockedFlags(void);

    static void increase(Flags flags);
    static void decrease(Flags flags);
    static void temporary_increase(Flags flags);
    static void temporary_decrease(Flags flags);
    static void lock_increase(Flags flags);
    static void lock_decrease(Flags flags);
    static void lock_toggle(Flags flags);

    // lock_clear clears only Virtual locks (not hardware CapsLock).
    static void lock_clear(void) { lock_decrease(getLockedFlags()); }

    static void sticky_increase(Flags flags);
    static void sticky_decrease(Flags flags);
    static void sticky_toggle(Flags flags);
    static void sticky_clear(void);

  private:
    static void updateStatusMessage(void);

    static Flags statusMessageFlags_;
    static Item item_[MAXNUM];
  };
}

#endif
