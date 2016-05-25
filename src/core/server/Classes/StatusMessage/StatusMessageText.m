#import "StatusMessageText.h"
#import "PreferencesModel.h"
#include "bridge.h"

enum StatusMessageTextType {
  StatusMessageTextTypeExtra,
  StatusMessageTextTypeModifierLock,
  StatusMessageTextTypeCapsLock,
  StatusMessageTextTypeModifierSticky,
  StatusMessageTextTypePointingButtonLock,
  StatusMessageTextType_End_,
};

@interface StatusMessageText ()

@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property NSMutableArray* texts;

@end

@implementation StatusMessageText

- (instancetype)init {
  self = [super init];

  if (self) {
    _texts = [NSMutableArray new];
    NSDictionary* dictionary = @{
      @"name" : @"",
      @"symbol" : @"",
    };
    for (int i = 0; i < StatusMessageTextType_End_; ++i) {
      [_texts addObject:[dictionary mutableCopy]];
    }
  }

  return self;
}

- (void)reset {
  for (int i = 0; i < StatusMessageTextType_End_; ++i) {
    self.texts[i][@"name"] = @"";
    self.texts[i][@"symbol"] = @"";
  }
}

- (void)updateText:(NSInteger)index text:(NSString*)text {
  switch (index) {
  case BRIDGE_USERCLIENT_STATUS_MESSAGE_EXTRA:
    self.texts[StatusMessageTextTypeExtra][@"name"] = text;
    break;

  case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK_NAME:
    self.texts[StatusMessageTextTypeModifierLock][@"name"] = text;
    break;

  case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK_SYMBOL:
    self.texts[StatusMessageTextTypeModifierLock][@"symbol"] = text;
    break;

  case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_CAPS_LOCK_NAME:
    self.texts[StatusMessageTextTypeCapsLock][@"name"] = text;
    break;

  case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_CAPS_LOCK_SYMBOL:
    self.texts[StatusMessageTextTypeCapsLock][@"symbol"] = text;
    break;

  case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY_NAME:
    self.texts[StatusMessageTextTypeModifierSticky][@"name"] = text;
    break;

  case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY_SYMBOL:
    self.texts[StatusMessageTextTypeModifierSticky][@"symbol"] = text;
    break;

  case BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK:
    self.texts[StatusMessageTextTypePointingButtonLock][@"name"] = text;
    break;

  default:
    NSLog(@"Error: Invalid index in [StatusMessageText updateText:%d text:%@]", (int)(index), text);
    break;
  }
}

- (NSString*)extraText {
  return self.texts[StatusMessageTextTypeExtra][@"name"];
}

- (NSString*)modifierLockText {
  if (self.preferencesModel.useModifierSymbolsInStatusWindow) {
    if (self.preferencesModel.showCapsLockStateInStatusWindow) {
      return [NSString stringWithFormat:@"%@%@",
                                        self.texts[StatusMessageTextTypeCapsLock][@"symbol"],
                                        self.texts[StatusMessageTextTypeModifierLock][@"symbol"]];
    } else {
      return self.texts[StatusMessageTextTypeModifierLock][@"symbol"];
    }
  } else {
    if (self.preferencesModel.showCapsLockStateInStatusWindow) {
      return [NSString stringWithFormat:@"%@%@",
                                        self.texts[StatusMessageTextTypeCapsLock][@"name"],
                                        self.texts[StatusMessageTextTypeModifierLock][@"name"]];
    } else {
      return self.texts[StatusMessageTextTypeModifierLock][@"name"];
    }
  }
}

- (NSString*)modifierStickyText {
  if (!self.preferencesModel.showStickyModifiersStateInStatusWindow) {
    return @"";
  }

  if (self.preferencesModel.useModifierSymbolsInStatusWindow) {
    return self.texts[StatusMessageTextTypeModifierSticky][@"symbol"];
  } else {
    return self.texts[StatusMessageTextTypeModifierSticky][@"name"];
  }
}

- (NSString*)pointingButtonLockText {
  if (!self.preferencesModel.showPointingButtonLockStateInStatusWindow) {
    return @"";
  }

  return self.texts[StatusMessageTextTypePointingButtonLock][@"name"];
}

@end
