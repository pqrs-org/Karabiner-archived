// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface PreferencesController : NSObject {
  IBOutlet NSWindow* preferencesWindow_;
  IBOutlet org_pqrs_KeyRemap4MacBook_Client* client_;

  IBOutlet NSButton* startAtLogin_;

  IBOutlet NSButton* targetSettingIsEnabled1_;
  IBOutlet NSButton* targetSettingIsEnabled2_;
  IBOutlet NSButton* targetSettingIsEnabled3_;

  IBOutlet NSTextField* targetSetting1_;
  IBOutlet NSTextField* targetSetting2_;
  IBOutlet NSTextField* targetSetting3_;
}

- (void) load;
- (void) show;

- (IBAction) setStartAtLogin:(id)sender;
- (IBAction) set:(id)sender;

@end
