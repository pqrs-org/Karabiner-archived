#ifndef IGNOREMULTIPLESAMEKEYPRESS_HPP
#define IGNOREMULTIPLESAMEKEYPRESS_HPP

#include "FromEvent.hpp"
#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    // ex. Ignore JIS_KANA x 2. (validate only the first once)
    class IgnoreMultipleSameKeyPress {
    public:
      IgnoreMultipleSameKeyPress(void);
      ~IgnoreMultipleSameKeyPress(void);

      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey_
      void add(AddDataType datatype, AddValue newval);

    private:
      FromEvent fromEvent_;
      Vector_ModifierFlag fromModifierFlags_;
      bool needToIgnore_;
    };
  }
}

#endif
