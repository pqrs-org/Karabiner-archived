#include "BlockUntilKeyUp.hpp"
#include "FlagStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    BlockUntilKeyUp::add(AddDataType datatype, AddValue newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
          fromEvent_ = FromEvent(datatype, newval);
          break;

        case BRIDGE_DATATYPE_MODIFIERFLAG:
        case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
          fromModifierFlags_.push_back(ModifierFlag(datatype, newval));
          break;

        default:
          IOLOG_ERROR("BlockUntilKeyUp::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
          break;
      }
    }

    bool
    BlockUntilKeyUp::isTargetEvent(const ParamsUnion& paramsUnion)
    {
      return fromEvent_.changePressingState(paramsUnion,
                                            FlagStatus::globalFlagStatus(),
                                            fromModifierFlags_);
    }
  }
}
