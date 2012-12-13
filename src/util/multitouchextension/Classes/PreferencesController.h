// -*- Mode: objc -*-

#import <Cocoa/Cocoa.h>
#import "KeyRemap4MacBookClient.h"

@interface PreferencesController : NSObject {
  NSMutableArray* oldSettings_;

  IBOutlet NSWindow* preferencesWindow_;
  IBOutlet KeyRemap4MacBookClient* client_;

  IBOutlet NSButton* startAtLogin_;
}

- (void) load;
- (void) show;
+ (BOOL) isSettingEnabled:(NSInteger)fingers;
+ (NSString*) getSettingName:(NSInteger)fingers;

- (IBAction) setStartAtLogin:(id)sender;
- (IBAction) set:(id)sender;

@end
