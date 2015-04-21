#ifndef REMAPFUNCFACTORY_HPP
#define REMAPFUNCFACTORY_HPP

#include "bridge.h"
#include "BlockUntilKeyUp.hpp"
#include "DoublePressModifier.hpp"
#include "DropAllKeys.hpp"
#include "DropKeyAfterRemap.hpp"
#include "DropPointingRelativeCursorMove.hpp"
#include "DropScrollWheel.hpp"
#include "FlipPointingRelative.hpp"
#include "FlipScrollWheel.hpp"
#include "ForceNumLockOn.hpp"
#include "HoldingKeyToKey.hpp"
#include "IgnoreMultipleSameKeyPress.hpp"
#include "KeyDownUpToKey.hpp"
#include "KeyOverlaidModifier.hpp"
#include "KeyToKey.hpp"
#include "PassThrough.hpp"
#include "PointingRelativeToKey.hpp"
#include "PointingRelativeToScroll.hpp"
#include "ScrollWheelToKey.hpp"
#include "ScrollWheelToScrollWheel.hpp"
#include "SetKeyboardType.hpp"
#include "SimultaneousKeyPresses.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class RemapFuncFactory final {
public:
  static RemapFuncBase* create(const unsigned int* vec, size_t length, AutogenId autogenId) {
    // ------------------------------------------------------------
    // check parameters.
    //
    if (!vec || length == 0) {
      IOLOG_ERROR("RemapFuncFactory::create invalid parameter %p, %ld.\n", vec, length);
      return nullptr;
    }

    // ------------------------------------------------------------
    unsigned int type = vec[0];

    RemapFuncBase* p = nullptr;

    switch (type) {
    case BRIDGE_REMAPTYPE_KEYTOKEY:
      p = new KeyToKey(autogenId);
      break;
    case BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER:
      p = new DoublePressModifier(autogenId);
      break;
    case BRIDGE_REMAPTYPE_DROPALLKEYS:
      p = new DropAllKeys(autogenId);
      break;
    case BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP:
      p = new DropKeyAfterRemap(autogenId);
      break;
    case BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE:
      p = new DropPointingRelativeCursorMove(autogenId);
      break;
    case BRIDGE_REMAPTYPE_DROPSCROLLWHEEL:
      p = new DropScrollWheel(autogenId);
      break;
    case BRIDGE_REMAPTYPE_FLIPPOINTINGRELATIVE:
      p = new FlipPointingRelative(autogenId);
      break;
    case BRIDGE_REMAPTYPE_FLIPSCROLLWHEEL:
      p = new FlipScrollWheel(autogenId);
      break;
    case BRIDGE_REMAPTYPE_FORCENUMLOCKON:
      p = new ForceNumLockOn(autogenId);
      break;
    case BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY:
      p = new HoldingKeyToKey(autogenId);
      break;
    case BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS:
      p = new IgnoreMultipleSameKeyPress(autogenId);
      break;
    case BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER:
      p = new KeyOverlaidModifier(autogenId);
      break;
    case BRIDGE_REMAPTYPE_KEYDOWNUPTOKEY:
      p = new KeyDownUpToKey(autogenId);
      break;
    case BRIDGE_REMAPTYPE_POINTINGRELATIVETOKEY:
      p = new PointingRelativeToKey(autogenId);
      break;
    case BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL:
      p = new PointingRelativeToScroll(autogenId);
      break;
    case BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES:
      p = new SimultaneousKeyPresses(autogenId);
      break;
    case BRIDGE_REMAPTYPE_SETKEYBOARDTYPE:
      p = new SetKeyboardType(autogenId);
      break;
    case BRIDGE_REMAPTYPE_SCROLLWHEELTOSCROLLWHEEL:
      p = new ScrollWheelToScrollWheel(autogenId);
      break;
    case BRIDGE_REMAPTYPE_SCROLLWHEELTOKEY:
      p = new ScrollWheelToKey(autogenId);
      break;
    case BRIDGE_REMAPTYPE_BLOCKUNTILKEYUP:
      p = new BlockUntilKeyUp(autogenId);
      break;
    case BRIDGE_REMAPTYPE_PASSTHROUGH:
      p = new PassThrough(autogenId);
      break;
    default:
      IOLOG_ERROR("RemapFuncFactory::create unknown type: %d\n", type);
      return nullptr;
    }

    if (!p) {
      IOLOG_ERROR("RemapFuncFactory::create failed to allocate: %d\n", type);
      return nullptr;
    }

    // ------------------------------------------------------------
    for (size_t i = 1;; i += 2) {
      size_t datatype_index = i;
      size_t value_index = i + 1;
      if (value_index >= length) break;

      if (vec[datatype_index] == BRIDGE_DATATYPE_OPTION &&
          Option(vec[value_index]) == Option::IGNORE_PASSTHROUGH) {
        p->setIgnorePassThrough(true);
      } else {
        p->add(AddDataType(vec[datatype_index]), AddValue(vec[value_index]));
      }
    }

    return p;
  }
};
}
}

#endif
