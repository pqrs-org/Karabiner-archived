#ifndef SIMULTANEOUSKEYPRESSES_HPP
#define SIMULTANEOUSKEYPRESSES_HPP

#include "RemapFuncBase.hpp"
#include "EventInputQueue.hpp"
#include "FromKeyChecker.hpp"
#include "KeyToKey.hpp"
#include "KeyToPointingButton.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SimultaneousKeyPresses {
    public:
      SimultaneousKeyPresses(void);
      ~SimultaneousKeyPresses(void);

      // This function changes Simultaneous key presses to KeyCode::VK_SIMULTANEOUSKEYPRESSES_xxx
      // It returns true if EventInputQueue::queue_ is changed.
      bool remap(void);
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
      void push_remapped(bool isKeyDown);

      class FromInfo {
      public:
        FromInfo(void) : active_(false) {}

        enum FromType {
          FROMTYPE_KEY,
          FROMTYPE_BUTTON,
        };
        void set(KeyCode k)        { type_ = FROMTYPE_KEY;    key_ = k; }
        void set(PointingButton b) { type_ = FROMTYPE_BUTTON; button_ = b; }
        void activate(void)        { active_ = true; }
        void deactivate(void)      { active_ = false; }

        bool isActive(void) const { return active_; }
        bool isTargetKeyDown(const EventInputQueue::Item& item) const;
        bool isTargetKeyUp(const EventInputQueue::Item& item) const;

      private:
        bool isTarget(bool& isKeyDown, const EventInputQueue::Item& item) const;

        FromType type_;
        KeyCode key_;
        PointingButton button_;
        bool active_;
      };

      // --------------------
      size_t index_;
      KeyCode virtualkey_;

      FromInfo fromInfo_[2];
      Flags fromFlags_;

      // --------------------
      enum ToType {
        TOTYPE_NONE,
        TOTYPE_KEY,
        TOTYPE_BUTTON,
      } toType_;

      bool isToRaw_;
      KeyCode toKey_raw_;

      KeyToKey keytokey_;
      KeyToPointingButton keytopointingbutton_;
    };
  }
}

#endif
