#ifndef KEYUPEVENTTOKEY_HPP
#define KEYUPEVENTTOKEY_HPP

#include "KeyToKey.hpp"
#include "List.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class KeyUpEventToKey {
    public:
      static void static_initialize(void);
      static void static_terminate(void);

      KeyUpEventToKey(void) : preserveKeyDownOrder_(false) {}

      bool remap(RemapParams& remapParams);

      // ========================================
      // __KeyUpEventToKey__ syntax
      //
      // <autogen>
      //   __KeyUpEventToKey__
      //   @begin
      //   FromEvent, FromFlags,
      //   ToKeys_when_KeyDown,
      //   @end
      //
      //   @begin
      //   KeyUpFlags,
      //   ToKeys_when_KeyUp,
      //   @end
      //
      //   ...
      //
      //   @begin
      //   KeyUpFlags,
      //   ToKeys_when_KeyUp,
      //   @end
      //
      // </autogen>
      //
      //
      // * You can omit FromFlags, ToKeys_when_KeyDown, KeyUpFlags.
      //
      // ========================================
      // Example:
      //
      // <autogen>
      //   __KeyUpEventToKey__
      //   @begin
      //   KeyCode::A, ModifierFlag::SHIFT_L,    (effective when A key is pressed with left shift key.)
      //   @end
      //
      //   @begin
      //   ModifierFlag::SHIFT_L                 (when A key is released with left shift key.)
      //   KeyCode::ESCAPE,                      (send escape key.)
      //   @end
      //
      //   @begin
      //   KeyCode::A                            (when A key is released, send A key.)
      //   @end
      // </autogen>
      //

      void add(AddDataType datatype, AddValue newval);

      class Item : public List::Item {
      public:
        Item(KeyUpEventToKey& k) : keyUpEventToKey_(k) {}
        KeyUpEventToKey& get(void) { return keyUpEventToKey_; }

      private:
        KeyUpEventToKey& keyUpEventToKey_;
      };

    private:
      void doKeyUp(void);

      KeyToKey fromKeyToKey_;
      Vector_KeyToKey toKeyToKeys_;
      bool preserveKeyDownOrder_;

      // Queue for Option::KEYUPEVENTTOKEY_PRESERVE_KEYDOWN_ORDER.
      static List queue_;
    };
  }
}

#endif
