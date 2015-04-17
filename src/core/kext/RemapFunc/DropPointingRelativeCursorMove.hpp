#ifndef DROPPOINTINGRELATIVECURSORMOVE_HPP
#define DROPPOINTINGRELATIVECURSORMOVE_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropPointingRelativeCursorMove final : public RemapFuncBase {
public:
  DropPointingRelativeCursorMove(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE, autogenId) {}

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override {}
};
}
}

#endif
