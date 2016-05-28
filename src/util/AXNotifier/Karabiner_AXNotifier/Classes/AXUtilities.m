#import "AXUtilities.h"
#import "GlobalAXNotifierPreferencesModel.h"

@implementation AXUtilities

+ (AXUIElementRef)copyFocusedUIElement {
  if (!AXIsProcessTrusted()) return NULL;

  AXUIElementRef result = NULL;
  AXUIElementRef systemWideElement = NULL;
  AXError error = kAXErrorSuccess;

  systemWideElement = AXUIElementCreateSystemWide();
  if (!systemWideElement) goto finish;

  error = AXUIElementCopyAttributeValue(systemWideElement,
                                        kAXFocusedUIElementAttribute,
                                        (CFTypeRef*)(&result));
  if (error != kAXErrorSuccess) {
    result = NULL;
    goto finish;
  }

finish:
  if (systemWideElement) {
    CFRelease(systemWideElement);
    systemWideElement = NULL;
  }
  return result;
}

+ (AXUIElementRef)copyFocusedWindow:(AXUIElementRef)applicationElement {
  AXUIElementRef result = NULL;
  AXError error = AXUIElementCopyAttributeValue(applicationElement,
                                                kAXFocusedWindowAttribute,
                                                (CFTypeRef*)(&result));
  if (error != kAXErrorSuccess) {
    if ([GlobalAXNotifierPreferencesModel debuggingLogEnabled]) {
      NSLog(@"copyFocusedWindow is failed. error:%@", [AXUtilities errorString:error]);
    }
    return NULL;
  }
  return result;
}

+ (NSArray*)attributeNamesOfUIElement:(AXUIElementRef)element {
  if (!element) return nil;

  CFArrayRef attrNames = NULL;

  AXUIElementCopyAttributeNames(element, &attrNames);
  if (!attrNames) return nil;

  return (__bridge_transfer NSArray*)(attrNames);
}

+ (id)valueOfAttribute:(NSString*)attribute ofUIElement:(AXUIElementRef)element {
  if (!attribute) return nil;
  if (!element) return nil;

  NSArray* attributeNames = [AXUtilities attributeNamesOfUIElement:element];

  if (!attributeNames) return nil;
  if ([attributeNames indexOfObject:attribute] == NSNotFound) return nil;

  CFTypeRef result = NULL;
  if (AXUIElementCopyAttributeValue(element, (__bridge CFStringRef)(attribute), &result) != kAXErrorSuccess) {
    return nil;
  }
  if (!result) return nil;

  return (__bridge_transfer id)(result);
}

+ (NSString*)titleOfUIElement:(AXUIElementRef)element {
  if (!element) return nil;
  return (NSString*)[AXUtilities valueOfAttribute:NSAccessibilityTitleAttribute ofUIElement:element];
}

+ (NSString*)roleOfUIElement:(AXUIElementRef)element {
  if (!element) return nil;
  return (NSString*)[AXUtilities valueOfAttribute:NSAccessibilityRoleAttribute ofUIElement:element];
}

+ (NSString*)subroleOfUIElement:(AXUIElementRef)element {
  if (!element) return nil;
  return (NSString*)[AXUtilities valueOfAttribute:NSAccessibilitySubroleAttribute ofUIElement:element];
}

+ (NSString*)errorString:(AXError)error {
  switch (error) {
#define TO_STRING(NAME) \
  case NAME:            \
    return @ #NAME;

    TO_STRING(kAXErrorSuccess);
    TO_STRING(kAXErrorFailure);
    TO_STRING(kAXErrorIllegalArgument);
    TO_STRING(kAXErrorInvalidUIElement);
    TO_STRING(kAXErrorInvalidUIElementObserver);
    TO_STRING(kAXErrorCannotComplete);
    TO_STRING(kAXErrorAttributeUnsupported);
    TO_STRING(kAXErrorActionUnsupported);
    TO_STRING(kAXErrorNotificationUnsupported);
    TO_STRING(kAXErrorNotImplemented);
    TO_STRING(kAXErrorNotificationAlreadyRegistered);
    TO_STRING(kAXErrorNotificationNotRegistered);
    TO_STRING(kAXErrorAPIDisabled);
    TO_STRING(kAXErrorNoValue);
    TO_STRING(kAXErrorParameterizedAttributeUnsupported);
    TO_STRING(kAXErrorNotEnoughPrecision);
  }
  return @"Unknown error";
}

@end
