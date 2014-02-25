#ifndef POINTINGRELATIVETOKEY_HPP
#define POINTINGRELATIVETOKEY_HPP

#include "RemapFuncClasses.hpp"
#include "KeyToKey.hpp"
#include "IntervalChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class PointingRelativeToKey {
    public:
      PointingRelativeToKey(void);
      ~PointingRelativeToKey(void);

      bool remap(RemapParams& remapParams);

      void add(unsigned int datatype, unsigned int newval);

    private:
      Flags fromFlags_;

      bool isFirstEvent;
      IntervalChecker keyrepeat_ic_;

      enum {
        KEYTOKEY_UP,
        KEYTOKEY_DOWN,
        KEYTOKEY_LEFT,
        KEYTOKEY_RIGHT,
        KEYTOKEY__END__,
      };
      KeyToKey keytokey_[KEYTOKEY__END__];
      KeyToKey* current_keytokey_;
    };
  }
}

#endif
