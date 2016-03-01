// -*- Mode: objc -*-

@import Cocoa;

@interface AppDelegate : NSObject <NSApplicationDelegate>

- (void)updateFocusedUIElementInformation:(NSDictionary*)information;
- (NSDictionary*)getFocusedUIElementInformation;
- (NSArray*)getWorkspaceAppIds;
- (NSArray*)getWorkspaceWindowNameIds;
- (NSArray*)getWorkspaceInputSourceIds;
- (NSDictionary*)getInputSourceInformation;

+ (void)quitWithConfirmation;

@end
