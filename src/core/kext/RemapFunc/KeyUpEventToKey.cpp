#include "EventWatcher.hpp"
#include "KeyUpEventToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    KeyUpEventToKey::add(unsigned int datatype, unsigned int newval)
    {
      if (datatype == BRIDGE_DATATYPE_OPTION &&
          Option(newval) == Option::SEPARATOR) {
        toKeyToKeys_.push_back(KeyToKey());
        toKeyToKeys_.back().add(KeyCode::VK_PSEUDO_KEY);

      } else {
        if (toKeyToKeys_.empty()) {
          fromKeyToKey_.add(datatype, newval);
        } else {
          toKeyToKeys_.back().add(datatype, newval);
        }
      }
    }

    bool
    KeyUpEventToKey::remap(RemapParams& remapParams)
    {
      if (remapParams.isremapped) return false;
      if (! fromKeyToKey_.remap(remapParams)) return false;

      bool iskeydown;
      if (remapParams.paramsUnion.iskeydown(iskeydown)) {
        if (iskeydown) {
          EventWatcher::undo();
        } else {
          EventWatcher::on();

          for (size_t i = 0; i < toKeyToKeys_.size(); ++i) {
            if (toKeyToKeys_[i].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN)) {
              toKeyToKeys_[i].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
              break;
            }
          }
        }
      }
      return true;
    }
  }
}
