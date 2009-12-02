#ifndef EVENTWATCHER_HPP
#define EVENTWATCHER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class EventWatcher {
    friend class Params_KeyboardEventCallBack;
    friend class Params_KeyboardSpecialEventCallback;
    friend class Params_RelativePointerEventCallback;
    friend class Params_ScrollWheelEventCallback;

  public:
    static void initialize(void);
    static void terminate(void);

    static void reset(void);

    static void set(bool& b);
    static void unset(bool& b);

    static void countup(void);

  private:
    static void on(void);

    enum {
      MAXNUM = 32,
    };
    struct Item {
      bool* flag;
      int count;
    };
    static Item item_[MAXNUM];
    static IOLock* lock_;

    // A variable to ignore the event in the sequence that we called EventWatcher::set.
    static int count_;
  };
}

#endif
