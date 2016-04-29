// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class PreferencesModel;

@interface AXApplicationObserverManager : NSObject

- (instancetype)initWithPreferencesModel:(PreferencesModel*)preferencesModel;

@end
