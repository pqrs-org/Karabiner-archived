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
                                        // Use AutogenId::maxValue for interrupted keys because interrupted keys are not changed by Karabiner.
                                        // That needs for __DropAllKeys__ with __KeyDownUpToKey__.
                                        //
                                        // For example, when we use `remap.launcher_mode_v2`,
                                        // [O down, A down, Z down] should be canceled by __DropAllKeys__.
                                        //
                                        // We need to send A,Z (interrupted keys) with AutogenId::maxValue() to cancel them in __DropAllKeys__.
                                        keytokey_interrupted_(AutogenId::maxValue()),
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
