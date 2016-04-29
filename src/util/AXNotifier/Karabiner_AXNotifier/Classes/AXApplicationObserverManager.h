// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class AXNotifierPreferencesModel;

@interface AXApplicationObserverManager : NSObject

- (instancetype)initWithAXNotifierPreferencesModel:(AXNotifierPreferencesModel*)axNotifierPreferencesModel;

@end
