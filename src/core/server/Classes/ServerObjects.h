/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

@import Cocoa;

@class ClientForKernelspace;
@class PreferencesManager;
@class StatusMessageManager;
@class Updater;
@class XMLCompiler;

@interface ServerObjects : NSObject
@property(weak) IBOutlet ClientForKernelspace* clientForKernelspace;
@property(weak) IBOutlet PreferencesManager* preferencesManager;
@property(weak) IBOutlet StatusMessageManager* statusMessageManager;
@property(weak) IBOutlet Updater* updater;
@property(weak) IBOutlet XMLCompiler* xmlCompiler;
@end
