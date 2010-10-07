#ifndef POINTINGBUTTONTOKEY_HPP
#define POINTINGBUTTONTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class PointingButtonToKey {
    public:
      PointingButtonToKey(void) : index_(0) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapPointingParams_relative& remapParams);

      // ----------------------------------------
      // [0] => fromButton_
      // [1] => toKeys_[0]
      // [2] => toKeys_[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairPointingButtonFlags fromButton_;

      KeyToKey keytokey_;
    };
  }
}

#endif
