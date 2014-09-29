#include "CallbackWrapper.hpp"

namespace org_pqrs_Karabiner {
  Params_Base emptyInstance_;

  const Params_Base&
  Params_Base::emptyInstance(void)
  {
    return emptyInstance_;
  }
}
