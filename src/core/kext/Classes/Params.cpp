#include "Params.hpp"

namespace org_pqrs_Karabiner {
namespace {
Params_Base emptyInstance_;
}

const Params_Base&
Params_Base::emptyInstance(void) {
  return emptyInstance_;
}
}
