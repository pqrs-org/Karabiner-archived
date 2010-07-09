#ifndef BUTTONSTATUS_HPP
#define BUTTONSTATUS_HPP

#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ButtonStatus {
  public:
    class Item {
      friend class ButtonStatus;

    public:
      Item(void) : button_(0) {}

    private:
      void initialize(PointingButton b) {
        button_ = b;
        count_ = 0;
      }
      void set(PointingButton b, bool isbuttondown) {
        if (button_ != b) return;

        if (isbuttondown) {
          ++count_;
        } else {
          --count_;
        }
      }

      void reset(void) { count_ = 0; }

      PointingButton makeButtons(void) const {
        if (count_ > 0) {
          return button_;
        } else {
          return 0;
        }
      }

      void increase(void) { ++count_; }
      void decrease(void) { --count_; }

      PointingButton button_;
      int count_;
    };
    enum { MAXNUM = 32 };

    static bool initialize(void);
    static void set(PointingButton button, bool isbuttondown);
    static Buttons makeButtons(void);
    static void reset(void);

    static void increase(Buttons buttons);
    static void decrease(Buttons buttons);

  private:
    static Item item_[MAXNUM];
  };
}

#endif
