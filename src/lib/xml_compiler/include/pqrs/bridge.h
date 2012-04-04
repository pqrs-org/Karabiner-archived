#ifndef BRIDGE_H
#define BRIDGE_H

#include <sys/types.h>
#include <mach/mach_types.h>

enum {
  // Version 1: initial version
  // Version 2: Added BRIDGE_DATATYPE_DEVICEVENDOR,BRIDGE_DATATYPE_DEVICEPRODUCT,BRIDGE_REMAPTYPE_FORCENUMLOCKON
  // Version 3: Removed some essential configurations. (parameter.statuswindow_alpha_font, ...)
  // Version 4: Added an essential configuration. (general.suppress_growl_warning)
  // Version 5: Changed initialize vector format
  // Version 6: Added VK_MOUSEKEY_BUTTON_*
  // Version 7: Added essential configurations. (option.pointing_reverse_*_scroll)
  // Version 8: Removed an essential configuration. (general.show_sticky_modifier_status)
  // Version 9: Added VK_STICKY_*_FORCE_ON, VK_STICKY_*_FORCE_OFF.
  // Version 10: Added an essential configuration. (parameter.keyoverlaidmodifier_initial_modifier_wait)
  // Version 11: Changed initialize vector format (added configindex)
  // Version 12: Added BRIDGE_REMAPTYPE_DROPSCROLLWHEEL
  // Version 13: Added an essential configuration. (general.dont_remap_non_apple_keyboard)
  // Version 14: Removed an essential configuration. (general.capslock_led_hack)
  // Version 15: Added VK_CONSUMERKEY_*.
  // Version 16: Added BRIDGE_REMAPTYPE_STRIPMODIFIERFROMSCROLLWHEEL.
  // Version 17: Added VK_MOUSEKEY_BUTTON_BUTTON9 - VK_MOUSEKEY_BUTTON_BUTTON16
  // Version 18: Added an essential configuration. (general.dont_remap_any_pointing)
  // Version 19: Added BRIDGE_REMAPTYPE_SCROLLWHEELTOSCROLLWHEEL.
  //             (And removed BRIDGE_REMAPTYPE_STRIPMODIFIERFROMSCROLLWHEEL.)
  // Version 20: Added an essential configuration. (general.dont_remap_non_apple_pointing)
  // Version 21: Added ScrollWheel::*.
  // Version 22: Added KeyCode::VK_WAIT_*.
  // Version 23: Removed KeyCode::VK_WAIT_500MS
  // Version 24: Added KeyCode::VK_CHANGE_INPUTMODE_*.

  BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION = 24,
};

enum {
  BRIDGE_DATATYPE_NONE,
  BRIDGE_DATATYPE_KEYCODE,
  BRIDGE_DATATYPE_MODIFIERFLAG,
  BRIDGE_DATATYPE_FLAGS,
  BRIDGE_DATATYPE_CONSUMERKEYCODE,
  BRIDGE_DATATYPE_POINTINGBUTTON,
  BRIDGE_DATATYPE_SCROLLWHEEL,
  BRIDGE_DATATYPE_KEYBOARDTYPE,
  BRIDGE_DATATYPE_DEVICEVENDOR,
  BRIDGE_DATATYPE_DEVICEPRODUCT,
  BRIDGE_DATATYPE_OPTION,

  BRIDGE_REMAPTYPE_NONE,
  BRIDGE_REMAPTYPE_KEYTOKEY,
  BRIDGE_REMAPTYPE_KEYTOCONSUMER,
  BRIDGE_REMAPTYPE_KEYTOPOINTINGBUTTON,
  BRIDGE_REMAPTYPE_CONSUMERTOCONSUMER,
  BRIDGE_REMAPTYPE_CONSUMERTOKEY,
  BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER,
  BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP,
  BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE,
  BRIDGE_REMAPTYPE_DROPSCROLLWHEEL,
  BRIDGE_REMAPTYPE_FORCENUMLOCKON,
  BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY,
  BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS,
  BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER,
  BRIDGE_REMAPTYPE_POINTINGBUTTONTOKEY,
  BRIDGE_REMAPTYPE_POINTINGBUTTONTOPOINTINGBUTTON,
  BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL,
  BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES,
  BRIDGE_REMAPTYPE_SETKEYBOARDTYPE,
  BRIDGE_REMAPTYPE_SCROLLWHEELTOSCROLLWHEEL,
  BRIDGE_REMAPTYPE_SCROLLWHEELTOKEY,
  BRIDGE_REMAPTYPE_END,

  BRIDGE_FILTERTYPE_NONE,
  BRIDGE_FILTERTYPE_APPLICATION_NOT,
  BRIDGE_FILTERTYPE_APPLICATION_ONLY,
  BRIDGE_FILTERTYPE_CONFIG_NOT,
  BRIDGE_FILTERTYPE_CONFIG_ONLY,
  BRIDGE_FILTERTYPE_MODIFIER_NOT,
  BRIDGE_FILTERTYPE_MODIFIER_ONLY,
  BRIDGE_FILTERTYPE_DEVICE_NOT,
  BRIDGE_FILTERTYPE_DEVICE_ONLY,
  BRIDGE_FILTERTYPE_INPUTMODE_NOT,
  BRIDGE_FILTERTYPE_INPUTMODE_ONLY,
  BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT,
  BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY,
  BRIDGE_FILTERTYPE_END,

  BRIDGE_STATUSMESSAGE,
  BRIDGE_VK_CONFIG,
};

// ------------------------------------------------------------
// UserClient definitions.
enum {
  BRIDGE_USERCLIENT_OPEN,
  BRIDGE_USERCLIENT_CLOSE,
  BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION,
  BRIDGE_USERCLIENT_NOTIFICATION_FROM_KEXT,
  BRIDGE_USERCLIENT__END__,
};

enum {
  BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_SUCCESS = 0,
  BRIDGE_USERCLIENT_SYNCHRONIZED_COMMUNICATION_RETURN_ERROR_GENERIC = 1,
};

enum {
  BRIDGE_USERCLIENT_TYPE_NONE,
  BRIDGE_USERCLIENT_TYPE_SET_REMAPCLASSES_INITIALIZE_VECTOR,
  BRIDGE_USERCLIENT_TYPE_SET_CONFIG,

  BRIDGE_USERCLIENT_TYPE_GET_STATUS_MESSAGE,
  BRIDGE_USERCLIENT_TYPE_SET_WORKSPACEDATA,
};

enum {
  BRIDGE_USERCLIENT_NOTIFICATION_TYPE_NONE,
  BRIDGE_USERCLIENT_NOTIFICATION_TYPE_STATUS_MESSAGE_UPDATED,
  BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE,
};

enum {
  BRIDGE_USERCLIENT_STATUS_MESSAGE_NONE,
  BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA,
  BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK,
  BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY,
  BRIDGE_USERCLIENT_STATUS_MESSAGE__END__,
};
enum {
  BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN = 128,
};

// 64bit alignment.
struct BridgeUserClientStruct {
  uint32_t type;
  uint32_t option;
  mach_vm_address_t data;
  mach_vm_size_t size; // size of data
};
// STATIC_ASSERT for sizeof(struct BridgeUserClientStruct).
// We need to make this value same in kext and userspace.
enum { STATIC_ASSERT__sizeof_BridgeUserClientStruct = 1 / (sizeof(struct BridgeUserClientStruct) == 24) };

struct BridgeWorkSpaceData {
  uint32_t applicationtype;
  uint32_t inputmode;
  uint32_t inputmodedetail;
};
enum { STATIC_ASSERT__sizeof_BridgeWorkSpaceData = 1 / (sizeof(struct BridgeWorkSpaceData) == 12) };

// remapclasses_initialize_vector format:
//
// base:
// { BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION, the_count_of_initialize_vector, [<initialize_vector>] }
//
// <initialize_vector>
//   { size, configindex, {[<remap>], {<filter>}} }
//
// <filter>
//   { size, BRIDGE_FILTERTYPE_XXX, values }
//
// <remap>
//   { size, BRIDGE_REMAPTYPE_XXX, values }
//
// --------------------
//
// Example of <base>:
//   { BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION, 3, initialize_vector1, initialize_vector2, initialize_vector3 }
//
// Example of <initialize_vector>:
//   {
//     19,
//     1044,
//     5,BRIDGE_REMAPTYPE_KEYTOKEY,BRIDGE_DATATYPE_KEYCODE,0,BRIDGE_DATATYPE_KEYCODE,11,
//     2,BRIDGE_FILTERTYPE_CONFIG_NOT,881,
//     2,BRIDGE_FILTERTYPE_CONFIG_NOT,882,
//     5,BRIDGE_REMAPTYPE_KEYTOKEY,BRIDGE_DATATYPE_KEYCODE,1,BRIDGE_DATATYPE_KEYCODE,56,
//   };
//
//   No remap version
//   {
//     1,
//     2321,
//   }
//
// Example of <filter>:
//   { 2,BRIDGE_FILTERTYPE_CONFIG_NOT,881 }
//
// Example of <remap>:
//   { 5,BRIDGE_REMAPTYPE_KEYTOKEY,BRIDGE_DATATYPE_KEYCODE,0,BRIDGE_DATATYPE_KEYCODE,11 };
//

#endif
