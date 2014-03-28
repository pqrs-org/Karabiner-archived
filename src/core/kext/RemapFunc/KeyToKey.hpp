#ifndef KEYTOKEY_HPP
#define KEYTOKEY_HPP

#include "FromEvent.hpp"
#include "RemapFuncClasses.hpp"
#include "ToEvent.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class KeyToKey {
    public:
      KeyToKey(void);
      ~KeyToKey(void);

      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromEvent_
      // [1] => toKeys_[0]
      // [2] => toKeys_[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

      // ----------------------------------------
      // utility functions
      void add(KeyCode newval) { add(BRIDGE_DATATYPE_KEYCODE, newval.get()); }
      void add(Flags newval)   { add(BRIDGE_DATATYPE_FLAGS,   newval.get()); }
      void add(Option newval)  { add(BRIDGE_DATATYPE_OPTION,  newval.get()); }

      bool call_remap_with_VK_PSEUDO_KEY(EventType eventType);

      size_t toKeysSize(void) const { return toKeys_.size(); }

    private:
      int getDelayUntilRepeat(void);
      int getKeyRepeat(void);

      class CurrentToEvent {
      public:
        enum Value {
          TOKEYS,
          BEFOREKEYS,
          AFTERKEYS,
        };
      };

      Vector_ToEvent& getCurrentToEvent(void) {
        switch (currentToEvent_) {
          case CurrentToEvent::TOKEYS:     return toKeys_;
          case CurrentToEvent::BEFOREKEYS: return beforeKeys_;
          case CurrentToEvent::AFTERKEYS:  return afterKeys_;
        }
      }

      size_t index_;

      FromEvent fromEvent_;
      Flags fromFlags_;

      Vector_ToEvent toKeys_;
      Vector_ToEvent beforeKeys_;
      Vector_ToEvent afterKeys_;
      CurrentToEvent::Value currentToEvent_;

      int keyboardRepeatID_;
      bool isRepeatEnabled_;

      int delayUntilRepeat_;
      int keyRepeat_;
    };
    DECLARE_VECTOR(KeyToKey);
  }
}

#endif
