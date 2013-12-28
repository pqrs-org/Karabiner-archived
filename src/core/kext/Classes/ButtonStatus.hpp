#ifndef BUTTONSTATUS_HPP
#define BUTTONSTATUS_HPP

#include "KeyCode.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  class ButtonStatus {
  public:
    class Item {
      friend class ButtonStatus;

    public:
      Item(void) :
        button_(0),
        count_(0),
        lock_count_(0)
      {}

    private:
      void initialize(PointingButton b) {
        button_ = b;
        count_ = 0;
        lock_count_ = 0;
      }
      void set(PointingButton b, bool isbuttondown) {
        if (button_ != b) return;

        if (isbuttondown) {
          ++count_;
        } else {
          --count_;
        }
      }

      void reset(void) {
        count_ = 0;

        // Preserve lock_count_.
        //
        // ButtonStatus::reset is called when NumHeldDownKeys == 0,
        // We need to remember lock status even if all key is released.
        // So, do not reset lock_count_ here.
      }

      PointingButton makeButtons(void) const {
        if (count_ + lock_count_ > 0) {
          return button_;
        } else {
          return PointingButton();
        }
      }

      void increase(void) { ++count_; }
      void decrease(void) { --count_; }
      void lock_increase(void) { lock_count_ = 1; }
      void lock_decrease(void) { lock_count_ = 0; }
      void lock_toggle(void)   { lock_count_ = ! lock_count_; }

      PointingButton button_;
      int count_;

      int lock_count_;
    };
    enum { MAXNUM = 32 };

    static bool initialize(void);
    static void set(PointingButton button, bool isbuttondown);
    static Buttons makeButtons(void);
    static void reset(void);

    static Buttons getLockedButtons(void);

    static void increase(Buttons buttons);
    static void increase(PointingButton button) { increase(Buttons(button)); }
    static void decrease(Buttons buttons);
    static void decrease(PointingButton button) { decrease(Buttons(button)); }
    static void lock_increase(Buttons buttons);
    static void lock_decrease(Buttons buttons);
    static void lock_toggle(Buttons buttons);
    static void lock_toggle(PointingButton button) { lock_toggle(Buttons(button)); }
    static void lock_clear(void) { lock_decrease(getLockedButtons()); }

  private:
    static void updateStatusMessage(void);

    static Buttons statusMessageButtons_[BRIDGE_USERCLIENT_STATUS_MESSAGE__END__];
    static Item item_[MAXNUM];
  };
}

#endif
