#ifndef FLAGSTATUS_HPP
#define FLAGSTATUS_HPP

#include "KeyCode.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  class FlagStatus {
  public:
    class Item {
      friend class FlagStatus;

    public:
      Item(void) :
        flag_(ModifierFlag::ZERO),
        count_(0),
        temporary_count_(0),
        lock_count_(0),
        sticky_count_(0)
      {}

    private:
      void initialize(ModifierFlag f);
      void set(void);
      void set(KeyCode key, Flags flags);

      void reset(void);
      int sum(void) const { return count_ + temporary_count_ + lock_count_ + sticky_count_; }
      Flags makeFlag(void) const {
        if (sum() > 0) {
          return Flags(flag_);
        } else {
          return Flags();
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
    DECLARE_VECTOR(Item);

    class ScopedTemporaryFlagsChanger {
    public:
      ScopedTemporaryFlagsChanger(FlagStatus& flagStatus, Flags toFlags) : flagStatus_(flagStatus) {
        for (size_t i = 0; i < flagStatus_.item_.size(); ++i) {
          count_.push_back(0);

          // ----------------------------------------
          // reset flag
          while (flagStatus_.item_[i].sum() < 0) {
            flagStatus_.item_[i].temporary_increase();
            ++(count_.back());
          }
          while (flagStatus_.item_[i].sum() > 0) {
            flagStatus_.item_[i].temporary_decrease();
            --(count_.back());
          }

          // ----------------------------------------
          // set a flag
          ModifierFlag flag = flagStatus_.getFlag(i);
          if (toFlags.isOn(flag)) {
            flagStatus_.item_[i].temporary_increase();
            ++(count_.back());
          }
        }
      }
      ~ScopedTemporaryFlagsChanger(void) {
        for (size_t i = 0; i < flagStatus_.item_.size(); ++i) {
          if (i >= count_.size()) break;

          while (count_[i] < 0) {
            flagStatus_.item_[i].temporary_increase();
            ++(count_[i]);
          }
          while (count_[i] > 0) {
            flagStatus_.item_[i].temporary_decrease();
            --(count_[i]);
          }
        }
      }

    private:
      FlagStatus& flagStatus_;
      Vector_int count_;
    };

    FlagStatus(void);

    void set(void);
    void set(KeyCode key, Flags flags);
    Flags makeFlags(void) const;
    // get registered ModifierFlag by index.
    ModifierFlag getFlag(size_t index) const;
    void reset(void);

    size_t itemSize(void) const { return item_.size(); }

    bool isOn(const Vector_ModifierFlag& modifierFlags) const;

    // getLockedFlags returns only Virtual locks (not hardware CapsLock).
    Flags getLockedFlags(void) const;
    Flags getStickyFlags(void) const;

    void increase(Flags flags);
    void decrease(Flags flags);
    void increase(ModifierFlag flag) { increase(Flags(flag)); }
    void decrease(ModifierFlag flag) { decrease(Flags(flag)); }
    void temporary_increase(Flags flags);
    void temporary_decrease(Flags flags);
    void temporary_increase(ModifierFlag flag) { temporary_increase(Flags(flag)); }
    void temporary_decrease(ModifierFlag flag) { temporary_decrease(Flags(flag)); }
    void lock_increase(Flags flags);
    void lock_increase(ModifierFlag flag) { lock_increase(Flags(flag)); }
    void lock_decrease(Flags flags);
    void lock_decrease(ModifierFlag flag) { lock_decrease(Flags(flag)); }
    void lock_toggle(Flags flags);
    void lock_toggle(ModifierFlag flag) { lock_toggle(Flags(flag)); }

    // lock_clear clears only Virtual locks (not hardware CapsLock).
    void lock_clear(void) { lock_decrease(getLockedFlags()); }

    void sticky_increase(Flags flags);
    void sticky_increase(ModifierFlag flag) { sticky_increase(Flags(flag)); }
    void sticky_decrease(Flags flags);
    void sticky_decrease(ModifierFlag flag) { sticky_decrease(Flags(flag)); }
    void sticky_toggle(Flags flags);
    void sticky_toggle(ModifierFlag flag) { sticky_toggle(Flags(flag)); }
    void sticky_clear(void);

    static FlagStatus& globalFlagStatus(void);

  private:
    bool isOn(ModifierFlag modifierFlag) const;
    void updateStatusMessage(void);

    Flags statusMessageFlags_[BRIDGE_USERCLIENT_STATUS_MESSAGE__END__];
    Vector_Item item_;
  };
}

#endif
