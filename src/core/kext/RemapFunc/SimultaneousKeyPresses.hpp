#ifndef SIMULTANEOUSKEYPRESSES_HPP
#define SIMULTANEOUSKEYPRESSES_HPP

#include "EventInputQueue.hpp"
#include "FromEvent.hpp"
#include "KeyToKey.hpp"
#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SimultaneousKeyPresses {
    public:
      SimultaneousKeyPresses(void);
      ~SimultaneousKeyPresses(void);

      // This function changes Simultaneous key presses to KeyCode::VK_SIMULTANEOUSKEYPRESSES_xxx
      // It returns true if EventInputQueue::queue_ is changed.
      bool remap(bool keyuponly);
      // This function changes KeyCode::VK_SIMULTANEOUSKEYPRESSES_xxx to remapped key/pointing events.
      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => virtualkey
      // [1] => fromKey1_
      // [2] => fromKey2_
      // [3] => toKeys_[0]
      // [4] => toKeys_[1]
      // [5] => ...
      void add(unsigned int datatype, unsigned int newval);

    private:
      void push_remapped(bool isKeyDown, const DeviceIdentifier& deviceIdentifier);

      class FromInfo {
      public:
        FromInfo(void)                                  : fromEvent_(),             active_(false) {}
        FromInfo(unsigned int datatype, unsigned int v) : fromEvent_(datatype, v),  active_(false) {}

        void activate(void)        { active_ = true; }
        void deactivate(void)      { active_ = false; }

        bool isActive(void) const { return active_; }

        const FromEvent& fromEvent(void) { return fromEvent_; }

      private:
        FromEvent fromEvent_;
        bool active_;
      };
      DECLARE_VECTOR(FromInfo);

      class DownKeys {
      public:
        DownKeys(void) : item(NULL) {}
        DownKeys(EventInputQueue::Item* i) : item(i) {}
        EventInputQueue::Item* item;
      };
      DECLARE_VECTOR(DownKeys);

      // --------------------
      bool isUseSeparator_;
      size_t index_;
      KeyCode virtualkey_;

      Vector_FromInfo fromInfo_;
      bool isFromInfoFull_;
      Flags fromFlags_;

      // --------------------
      bool isToRaw_;
      bool isStrictKeyOrder_;
      KeyCode toKey_raw_;

      KeyToKey keytokey_;

      Vector_DownKeys downKeys_;
    };
  }
}

#endif
