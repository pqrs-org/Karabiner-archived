// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@class PreferencesManager;

@interface GlobalDomainKeyRepeatObserver : NSObject

- (instancetype)initWithPreferencesManager:(PreferencesManager*)manager;
- (void)start;

@end
