// -*- Mode: objc -*-

@import Cocoa;

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property(copy, readonly) NSArray* workspaceAppIds;
@property(copy, readonly) NSArray* workspaceWindowNameIds;
@property(copy, readonly) NSArray* workspaceInputSourceIds;

- (void)updateFocusedUIElementInformation:(NSDictionary*)information;
- (NSDictionary*)getFocusedUIElementInformation;
- (NSDictionary*)getInputSourceInformation;

+ (void)quitWithConfirmation;

@end
