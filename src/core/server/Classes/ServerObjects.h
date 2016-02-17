/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@import Cocoa;

@class PreferencesManager;
@class XMLCompiler;

@interface ServerObjects : NSObject
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@end
