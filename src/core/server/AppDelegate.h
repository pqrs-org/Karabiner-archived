// -*- Mode: objc -*-

@import Cocoa;

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property(copy, readonly) NSDictionary* focusedUIElementInformation;
@property(copy, readonly) NSDictionary* inputSourceInformation;
@property(copy, readonly) NSArray* workspaceAppIds;
@property(copy, readonly) NSArray* workspaceWindowNameIds;
@property(copy, readonly) NSArray* workspaceInputSourceIds;

- (void)updateFocusedUIElementInformation:(NSDictionary*)information;

@end
