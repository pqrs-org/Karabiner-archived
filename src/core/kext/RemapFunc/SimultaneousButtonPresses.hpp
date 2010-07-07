#ifndef SIMULTANEOUSBUTTONPRESSES_HPP
#define SIMULTANEOUSBUTTONPRESSES_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SimultaneousButtonPresses {
    public:
      SimultaneousButtonPresses(void) : index_(0), active1_(false), active2_(false) {}
      void initialize(void);
      void terminate(void);
      void remap(void);

      // ----------------------------------------
      // [0] => fromButton1_
      // [1] => fromButton2_
      // [2] => toButton_
      void add(PointingButton newval);
      void add(Flags newval);

    private:
      void push_remapped(const Params_RelativePointerEventCallback& baseparams, bool isButtonDown);

      size_t index_;
      PointingButton fromButton1_;
      PointingButton fromButton2_;
      PointingButton toButton_;
      Flags fromFlags_;
      bool active1_;
      bool active2_;
    };
  }
}

#endif
