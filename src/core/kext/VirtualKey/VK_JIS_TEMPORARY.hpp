#ifndef VIRTUALKEY_VK_JIS_TEMPORARY_HPP
#define VIRTUALKEY_VK_JIS_TEMPORARY_HPP

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_JIS_TEMPORARY {
    public:
      static void initialize(IOWorkLoop& workloop);
      static void terminate(void);

      static bool handle(const Params_KeyboardEventCallBack& params);

    private:
      enum {
        KEYEVENT_DELAY_MS = 100,
      };
      static bool handle_core(const Params_KeyboardEventCallBack& params,
                              KeyCode key,
                              InputSourceDetail inputsourcedetail);
      static bool handle_RESTORE(const Params_KeyboardEventCallBack& params);

      static void firekeytoinputdetail(const Params_KeyboardEventCallBack& params,
                                       InputSourceDetail inputsourcedetail);

      static InputSourceDetail normalize(InputSourceDetail imd);

      static void fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender);
      static void fire(void);

      static InputSourceDetail savedinputsourcedetail_;
      static InputSourceDetail currentinputsourcedetail_;

      struct FireKeyInfo {
        Flags flags;
        KeyCode key;
        KeyboardType keyboardType;
        bool active;
      };
      static FireKeyInfo fireKeyInfo_;
      static TimerWrapper fire_timer_;
    };
  }
}

#endif
