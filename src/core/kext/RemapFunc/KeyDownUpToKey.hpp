#ifndef KEYDOWNUPTOKEY_HPP
#define KEYDOWNUPTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class KeyDownUpToKey final : public RemapFuncBase {
public:
  KeyDownUpToKey(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_KEYDOWNUPTOKEY, autogenId),
                                        indexType_(INDEXTYPE_NONE),
                                        keytokey_from_(autogenId),
                                        keytokey_downup_(autogenId),
                                        keytokey_interrupted_(autogenId),
                                        needClearInterruptedToKeys_(false),
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
  bool interrupted_;
};
}
}

#endif
