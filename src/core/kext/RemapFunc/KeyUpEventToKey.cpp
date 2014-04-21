#include "EventWatcher.hpp"
#include "KeyUpEventToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    List KeyUpEventToKey::queue_;

    void
    KeyUpEventToKey::static_initialize(void)
    {}

    void
    KeyUpEventToKey::static_terminate(void)
    {
      queue_.clear();
    }

    void
    KeyUpEventToKey::add(AddDataType datatype, AddValue newval)
    {
      if (datatype == BRIDGE_DATATYPE_OPTION &&
          Option(newval) == Option::KEYUPEVENTTOKEY_PRESERVE_KEYDOWN_ORDER) {
        preserveKeyDownOrder_ = true;

      } else if (datatype == BRIDGE_DATATYPE_OPTION &&
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

    void
    KeyUpEventToKey::doKeyUp(void)
    {
      EventWatcher::on();

      for (size_t i = 0; i < toKeyToKeys_.size(); ++i) {
        if (toKeyToKeys_[i].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN)) {
          toKeyToKeys_[i].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
          break;
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

          queue_.push_back(new Item(*this));

        } else {
          if (preserveKeyDownOrder_) {
            // Call doKeyUp of all items before myself.
            for (;;) {
              Item* p = static_cast<Item*>(queue_.front());
              if (! p) break;

              KeyUpEventToKey& keyUpEventToKey = p->get();
              queue_.pop_front();

              keyUpEventToKey.doKeyUp();
              if (&keyUpEventToKey == this) break;
            }

          } else {
            // Call doKeyUp of myself.
            for (Item* p = static_cast<Item*>(queue_.front()); p; p = static_cast<Item*>(p->getnext())) {
              KeyUpEventToKey& keyUpEventToKey = p->get();
              if (&keyUpEventToKey == this) {
                keyUpEventToKey.doKeyUp();
                queue_.erase_and_delete(p);
                break;
              }
            }
          }
        }
      }
      return true;
    }
  }
}
