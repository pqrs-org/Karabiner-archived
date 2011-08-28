#include "base.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_CONSUMERKEY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  RemapFunc::ConsumerToConsumer VirtualKey::VK_CONSUMERKEY::consumertoconsumer_[Index::END_];

  void
  VirtualKey::VK_CONSUMERKEY::initialize(void)
  {
    for (int i = 0; i < Index::END_; ++i) {
      consumertoconsumer_[i].add(ConsumerKeyCode::VK_PSEUDO_KEY);
      consumertoconsumer_[i].add(getConsumerKeyCode(static_cast<Index::Value>(i)));
    }
  }

  void
  VirtualKey::VK_CONSUMERKEY::terminate(void)
  {}

  bool
  VirtualKey::VK_CONSUMERKEY::handle(const Params_KeyboardEventCallBack& params)
  {
    Index::Value index = Index::END_;

    /**/ if (params.key == KeyCode::VK_CONSUMERKEY_BRIGHTNESS_DOWN)    { index = Index::BRIGHTNESS_DOWN;    }
    else if (params.key == KeyCode::VK_CONSUMERKEY_BRIGHTNESS_UP)      { index = Index::BRIGHTNESS_UP;      }
    else if (params.key == KeyCode::VK_CONSUMERKEY_KEYBOARDLIGHT_LOW)  { index = Index::KEYBOARDLIGHT_LOW;  }
    else if (params.key == KeyCode::VK_CONSUMERKEY_KEYBOARDLIGHT_HIGH) { index = Index::KEYBOARDLIGHT_HIGH; }
    else if (params.key == KeyCode::VK_CONSUMERKEY_MUSIC_PREV)         { index = Index::MUSIC_PREV;         }
    else if (params.key == KeyCode::VK_CONSUMERKEY_MUSIC_PLAY)         { index = Index::MUSIC_PLAY;         }
    else if (params.key == KeyCode::VK_CONSUMERKEY_MUSIC_NEXT)         { index = Index::MUSIC_NEXT;         }
    else if (params.key == KeyCode::VK_CONSUMERKEY_VOLUME_MUTE)        { index = Index::VOLUME_MUTE;        }
    else if (params.key == KeyCode::VK_CONSUMERKEY_VOLUME_DOWN)        { index = Index::VOLUME_DOWN;        }
    else if (params.key == KeyCode::VK_CONSUMERKEY_VOLUME_UP)          { index = Index::VOLUME_UP;          }
    else if (params.key == KeyCode::VK_CONSUMERKEY_EJECT)              { index = Index::EJECT;              }
    else if (params.key == KeyCode::VK_CONSUMERKEY_POWER)              { index = Index::POWER;              }

    if (index >= Index::END_) return false;

    if (params.repeat) return true;

    consumertoconsumer_[index].call_remap_with_VK_PSEUDO_KEY(params.eventType);

    return true;
  }

  ConsumerKeyCode
  VirtualKey::VK_CONSUMERKEY::getConsumerKeyCode(Index::Value index)
  {
    switch (index) {
      case Index::BRIGHTNESS_DOWN:    return ConsumerKeyCode::BRIGHTNESS_DOWN;
      case Index::BRIGHTNESS_UP:      return ConsumerKeyCode::BRIGHTNESS_UP;
      case Index::KEYBOARDLIGHT_LOW:  return ConsumerKeyCode::KEYBOARDLIGHT_LOW;
      case Index::KEYBOARDLIGHT_HIGH: return ConsumerKeyCode::KEYBOARDLIGHT_HIGH;
      case Index::MUSIC_PREV:         return ConsumerKeyCode::MUSIC_PREV;
      case Index::MUSIC_PLAY:         return ConsumerKeyCode::MUSIC_PLAY;
      case Index::MUSIC_NEXT:         return ConsumerKeyCode::MUSIC_NEXT;
      case Index::VOLUME_MUTE:        return ConsumerKeyCode::VOLUME_MUTE;
      case Index::VOLUME_DOWN:        return ConsumerKeyCode::VOLUME_DOWN;
      case Index::VOLUME_UP:          return ConsumerKeyCode::VOLUME_UP;
      case Index::EJECT:              return ConsumerKeyCode::EJECT;
      case Index::POWER:              return ConsumerKeyCode::POWER;
      case Index::END_:               return ConsumerKeyCode::VK_NONE;
    }
    return ConsumerKeyCode::VK_NONE;
  }
}
