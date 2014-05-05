#ifndef KEYOVERLAIDMODIFIER_HPP
#define KEYOVERLAIDMODIFIER_HPP

#include "FromEvent.hpp"
#include "RemapFuncClasses.hpp"
#include "DependingPressingPeriodKeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    // for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
    class KeyOverlaidModifier {
    public:
      KeyOverlaidModifier(void);
      ~KeyOverlaidModifier(void);

      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0]   => fromKey_
      // [1]   => toKey_
      // [2]   => toKeys_fire_[0]
      // [3]   => toKeys_fire_[1]
      // [4]   => toKeys_fire_[2]
      // [5]   => ...
      void add(AddDataType datatype, AddValue newval);

      // utility
      void add(KeyCode newval) { add(AddDataType(BRIDGE_DATATYPE_KEYCODE), AddValue(newval.get())); }
      void add(Option newval)  { add(AddDataType(BRIDGE_DATATYPE_OPTION),  AddValue(newval.get())); }

    private:
      bool isUseSeparator_;
      enum IndexType {
        INDEX_IS_HOLDING,
        INDEX_IS_NORMAL,
        INDEX_IS_REPEAT_TOKEYS,
      } indexType_;
      size_t index_;
      FromEvent fromEvent_;

      DependingPressingPeriodKeyToKey dppkeytokey_;
    };
  }
}

#endif
