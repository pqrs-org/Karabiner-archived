// -*- Mode: objc -*-

@import Cocoa;

@class ServerObjects;

@interface ParameterDiffCellView : NSTableCellView
@property(weak) ServerObjects* serverObjects;
@property(copy) NSString* settingIdentifier;
@property NSInteger defaultValue;

- (void)setObserver;
- (void)updateValue;
@end
