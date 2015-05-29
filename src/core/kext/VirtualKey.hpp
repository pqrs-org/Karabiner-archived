#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "KeyCode.hpp"
#include "Params.hpp"
#include "Types.hpp"

namespace org_pqrs_Karabiner {
namespace VirtualKey {
void initialize(IOWorkLoop& workloop);
void terminate(void);
void reset(void);
bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
bool handleAfterEnqueued(const Params_KeyboardEventCallBack& params);

bool isKeyLikeModifier(KeyCode keycode);
};
}

#endif
