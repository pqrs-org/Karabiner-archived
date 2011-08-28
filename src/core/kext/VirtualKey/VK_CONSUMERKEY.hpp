#ifndef VIRTUALKEY_VK_CONSUMERKEY_HPP
#define VIRTUALKEY_VK_CONSUMERKEY_HPP

#include "ConsumerToConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_CONSUMERKEY {
    public:
      static void initialize(void);
      static void terminate(void);
      static bool handle(const Params_KeyboardEventCallBack& params);

    private:
      class Index {
      public:
        enum Value {
          BRIGHTNESS_DOWN,
          BRIGHTNESS_UP,
          KEYBOARDLIGHT_LOW,
          KEYBOARDLIGHT_HIGH,
          MUSIC_PREV,
          MUSIC_PLAY,
          MUSIC_NEXT,
          VOLUME_MUTE,
          VOLUME_DOWN,
          VOLUME_UP,
          EJECT,
          POWER,
          END_,
        };
      };
      static RemapFunc::ConsumerToConsumer consumertoconsumer_[Index::END_];
      static ConsumerKeyCode getConsumerKeyCode(Index::Value index);
    };
  }
}

#endif
