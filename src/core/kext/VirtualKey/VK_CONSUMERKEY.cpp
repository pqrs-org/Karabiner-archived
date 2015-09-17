#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_CONSUMERKEY.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_CONSUMERKEY::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  ConsumerKeyCode key = ConsumerKeyCode::VK_NONE;

  /*  */ if (params.key == KeyCode::VK_CONSUMERKEY_BRIGHTNESS_DOWN) {
    key = ConsumerKeyCode::BRIGHTNESS_DOWN;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_BRIGHTNESS_UP) {
    key = ConsumerKeyCode::BRIGHTNESS_UP;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_KEYBOARDLIGHT_LOW) {
    key = ConsumerKeyCode::KEYBOARDLIGHT_LOW;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_KEYBOARDLIGHT_HIGH) {
    key = ConsumerKeyCode::KEYBOARDLIGHT_HIGH;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_MUSIC_PREV) {
    key = ConsumerKeyCode::MUSIC_PREV;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_MUSIC_PLAY) {
    key = ConsumerKeyCode::MUSIC_PLAY;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_MUSIC_NEXT) {
    key = ConsumerKeyCode::MUSIC_NEXT;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_VOLUME_MUTE) {
    key = ConsumerKeyCode::VOLUME_MUTE;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_VOLUME_DOWN) {
    key = ConsumerKeyCode::VOLUME_DOWN;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_VOLUME_UP) {
    key = ConsumerKeyCode::VOLUME_UP;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_EJECT) {
    key = ConsumerKeyCode::EJECT;
  } else if (params.key == KeyCode::VK_CONSUMERKEY_POWER) {
    key = ConsumerKeyCode::POWER;
  }

  if (key == ConsumerKeyCode::VK_NONE) return false;

  if (!key.isRepeatable() && params.repeat) return true;

  Params_KeyboardSpecialEventCallback p(params.eventType,
                                        params.flags,
                                        key,
                                        params.repeat);
  EventOutputQueue::FireConsumer::fire(p, autogenId, physicalEventType);

  return true;
}
}
