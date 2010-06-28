#ifndef DOUBLEPRESSMODIFIER_HPP
#define DOUBLEPRESSMODIFIER_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    // A modifier has DoublePressed key action.
    class DoublePressModifier {
    public:
      DoublePressModifier(void) : pressCount_(0) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey_
      // [1] => toKey_
      // [2] => fireKeys_[0]
      // [3] => fireKeys_[1]
      // [4] => fireKeys_[2]
      // ...
      void add(KeyCode newval);
      void add(Flags newval);

    private:
      size_t index_;
      int pressCount_;
      IntervalChecker ic_;
      PairKeyFlags fromKey_;

      KeyToKey keytokey_;
      KeyToKey keytokey_fire_;
    };
  }
}

#endif
