#ifndef FLAGSTATUS_HPP
#define FLAGSTATUS_HPP

#include "keycode.hpp"
#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class FlagStatus {
  public:
    class Item {
      friend class FlagStatus;

    public:
      void increase(void);
      void decrease(void);
      void temporary_increase(void) { ++temporary_count_; }
      void temporary_decrease(void) { --temporary_count_; }
      void lock_increase(void) { lock_count_ = 1; }
      void lock_decrease(void) { lock_count_ = 0; }

      bool isHeldDown(void) const { return (count_ + temporary_count_ + lock_count_ + original_lock_count_) > 0; }
      KeyCode::KeyCode getKeyCode(void) const { return key_; }

    private:
      void initialize(ModifierFlag::ModifierFlag f);
      void set(void);
      void set(const RemapParams& remapParams);

      void reset(void);
      unsigned int makeFlag(void) const { return (isHeldDown()) ? flag_ : 0; }

      ModifierFlag::ModifierFlag flag_;
      KeyCode::KeyCode key_;
      int count_;
      int temporary_count_;

      int lock_count_; // store remapped lock status. (CapsLock, FN lock, ...)
      int original_lock_count_; // store original CapsLock status.
    };

    static void initialize(void);
    static void set(void);
    static void set(const RemapParams& remapParams);
    static unsigned int makeFlags(unsigned int keyCode = KeyCode::VK_NONE);
    static unsigned int makeFlags(const RemapParams& remapParams);
    static void reset(void);

    static Item* getFlagStatus(ModifierFlag::ModifierFlag flag);
    static Item* getFlagStatus(KeyCode::KeyCode keyCode);

    static bool isHeldDown(ModifierFlag::ModifierFlag flag);
    static void increase(ModifierFlag::ModifierFlag flag);
    static void decrease(ModifierFlag::ModifierFlag flag);
    static void temporary_increase(ModifierFlag::ModifierFlag flag);
    static void temporary_decrease(ModifierFlag::ModifierFlag flag);
    static void lock_increase(ModifierFlag::ModifierFlag flag);
    static void lock_decrease(ModifierFlag::ModifierFlag flag);

  private:
    static Item item_[ModifierFlag::listsize];
  };
}

#endif
