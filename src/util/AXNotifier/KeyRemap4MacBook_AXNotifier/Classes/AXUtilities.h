// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface AXUtilities : NSObject

+ (AXUIElementRef) copyFocusedUIElement;
+ (AXUIElementRef) copyFocusedWindow;
+ (AXUIElementRef) copyFrontmost;
+ (AXUIElementRef) copyMainWindow;

+ (NSString*) titleOfUIElement:(AXUIElementRef)element;
+ (NSString*) roleOfUIElement:(AXUIElementRef)element;

@end
