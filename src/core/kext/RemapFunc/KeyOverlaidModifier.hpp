#ifndef KEYOVERLAIDMODIFIER_HPP
#define KEYOVERLAIDMODIFIER_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class KeyOverlaidModifier {
      // for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
    public:
      enum Option {
        OPTION_REPEAT,
      };

      static void static_initialize(IOWorkLoop& workloop);
      static void static_terminate(void);

      KeyOverlaidModifier(void) : index_(0), isAnyEventHappen_(false), savedflags_(0), isRepeatEnabled_(false) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0]   => fromKey_
      // [1]   => toKey_
      // [2]   => toKeys_fire_[0]
      // [3]   => toKeys_fire_[1]
      // [4]   => toKeys_fire_[2]
      // [5]   => ...
      void add(KeyCode newval);
      void add(Flags newval);
      void add(Option newval);

    private:
      static void firerepeat(OSObject* owner, IOTimerEventSource* sender);

      static TimerWrapper timer_;
      static KeyOverlaidModifier* target_;

      size_t index_;
      bool isAnyEventHappen_;
      PairKeyFlags toKey_;
      Flags savedflags_;
      IntervalChecker ic_;
      bool isRepeatEnabled_;
      bool isfirenormal_;
      bool isfirerepeat_;

      KeyToKey keytokey_;
      KeyToKey keytokey_fire_;
    };
  }
}

#endif
