// -*- Mode: objc -*-

@import Cocoa;

@class AXNotifierPreferencesModel;

@interface GlobalAXNotifierPreferencesModel : NSObject

+ (AXNotifierPreferencesModel*)get;
+ (void)set:(AXNotifierPreferencesModel*)newvalue;
+ (BOOL)debuggingLogEnabled;

@end
