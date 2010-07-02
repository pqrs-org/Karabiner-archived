#ifndef SIMULTANEOUSKEYPRESSES_HPP
#define SIMULTANEOUSKEYPRESSES_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SimultaneousKeyPresses {
    public:
      enum Option {
        OPTION_RAW,
      };

      SimultaneousKeyPresses(void) : index_(0), active1_(false), active2_(false) {}
      void initialize(void);
      void terminate(void);
      void remap(void);
      bool handlevirtualkey(const Params_KeyboardEventCallBack& params);

      // ----------------------------------------
      // [0] => virtualkey
      // [1] => fromKey1_
      // [2] => fromKey2_
      // [3] => toKeys_[0]
      // [4] => toKeys_[1]
      // [5] => ...
      void add(KeyCode newval);
      void add(Flags newval);
      void add(Option newval);

    private:
      void push_remapped(const Params_KeyboardEventCallBack& baseparams, bool isKeyDown);

      size_t index_;
      KeyCode virtualkey_;
      KeyCode fromKey1_;
      KeyCode fromKey2_;
      Flags fromFlags_;
      bool isToRawKey_;
      KeyCode rawToKey_;
      bool active1_;
      bool active2_;

      KeyToKey keytokey_;
    };
  }
}

#endif
