#ifndef DROPPOINTINGRELATIVECURSORMOVE_HPP
#define DROPPOINTINGRELATIVECURSORMOVE_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropPointingRelativeCursorMove final : public RemapFuncBase {
public:
  DropPointingRelativeCursorMove(void) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE) {}

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override {}
};
}
}

#endif
