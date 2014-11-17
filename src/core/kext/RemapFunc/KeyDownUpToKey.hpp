#ifndef KEYDOWNUPTOKEY_HPP
#define KEYDOWNUPTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class KeyDownUpToKey final : public RemapFuncBase {
public:
  KeyDownUpToKey(void) : RemapFuncBase(BRIDGE_REMAPTYPE_KEYDOWNUPTOKEY),
                         indexType_(INDEXTYPE_NONE),
                         needClearInterruptedToKeys_(false),
                         needProcessPrepare_(false),
                         interrupted_(false) {}

  void prepare(RemapParams& remapParams) override;
  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  enum IndexType {
    INDEXTYPE_NONE,
    INDEXTYPE_FROM,
    INDEXTYPE_DOWNUP,
    INDEXTYPE_INTERRUPTED,
  } indexType_;

  KeyToKey keytokey_from_;
  KeyToKey keytokey_downup_;
  KeyToKey keytokey_interrupted_;

  bool needClearInterruptedToKeys_;
  bool needProcessPrepare_;
  bool interrupted_;
};
}
}

#endif
