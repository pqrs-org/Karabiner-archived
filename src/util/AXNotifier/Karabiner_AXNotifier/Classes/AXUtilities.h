// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

@import Cocoa;

@interface AXUtilities : NSObject

+ (AXUIElementRef) copyFocusedUIElement;
+ (AXUIElementRef) copyFocusedWindow:(AXUIElementRef)applicationElement;

+ (NSString*) titleOfUIElement:(AXUIElementRef)element;
+ (NSString*) roleOfUIElement:(AXUIElementRef)element;

/*
 * About NSAccessibilitySubroleAttribute:
 *
 * We cannot use subrole to know whether user are editing text or not in web browser (eg. Google Chrome).
 * The subrole is same value (AXStandardWindow) in browser
 * even if the focused element is <textarea>, <input> or other elements.
 */
+ (NSString*) subroleOfUIElement:(AXUIElementRef)element;

@end
