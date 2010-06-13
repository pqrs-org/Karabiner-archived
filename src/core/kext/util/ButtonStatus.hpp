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
      void initialize(unsigned int b) {
        button_ = b;
        count_ = 0;
      }
      /**
       * Set count_ from difference of current/previous buttons.
       *
       * @param current current buttons
       * @param previous previous buttons
       *
       * @return button down count. (-1, 0, 1)
       */
      int set(Buttons current, Buttons previous) {
        bool iscurrent = current.isOn(PointingButton(button_));
        bool isprevious = previous.isOn(PointingButton(button_));

        if (iscurrent == isprevious) return 0;

        if (iscurrent) {
          ++count_;
          return 1;
        } else {
          --count_;
          return -1;
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
    /**
     * Set state from difference of current/previous buttons.
     *
     * @param current current buttons
     * @param previous previous buttons
     *
     * @return button down count. (may be negative value)
     */
    static int set(Buttons current, Buttons previous);
    static Buttons makeButtons(void);
    static void reset(void);

    static void increase(Buttons buttons);
    static void decrease(Buttons buttons);

  private:
    static Item item_[MAXNUM];
  };
}

#endif
