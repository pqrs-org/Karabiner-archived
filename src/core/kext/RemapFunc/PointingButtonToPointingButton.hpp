#ifndef POINTINGBUTTONTOPOINTINGBUTTON_HPP
#define POINTINGBUTTONTOPOINTINGBUTTON_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class PointingButtonToPointingButton {
    public:
      PointingButtonToPointingButton(void) : index_(0), toButtons_(NULL) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapPointingParams_relative& remapParams);

      // ----------------------------------------
      // [0] => fromButton_
      // [1] => toButtons_[0]
      // [2] => toButtons_[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairPointingButtonFlags fromButton_;
      Vector_PairPointingButtonFlags* toButtons_;
    };
  }
}

#endif
