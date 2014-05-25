#import "AXUtilities.h"

@implementation AXUtilities

+ (AXUIElementRef) copyUIElement:(CFStringRef)attribute
{
  if (! AXIsProcessTrusted()) return NULL;

  AXUIElementRef result = NULL;
  AXUIElementRef systemWideElement = NULL;
  AXUIElementRef applicationElement = NULL;
  AXUIElementRef baseElement = NULL;
  AXError error = kAXErrorSuccess;

  systemWideElement = AXUIElementCreateSystemWide();
  if (! systemWideElement) goto finish;

  // ----------------------------------------
  // set baseElement
  if (CFStringCompare(attribute, kAXFocusedUIElementAttribute, 0) == kCFCompareEqualTo) {
    // We can get kAXFocusedUIElementAttribute from systemWideElement.
    baseElement = systemWideElement;
  } else {
    error = AXUIElementCopyAttributeValue(systemWideElement,
                                          kAXFocusedApplicationAttribute,
                                          (CFTypeRef*)(&applicationElement));
    if (error != kAXErrorSuccess) {
      NSLog(@"AXUIElementCopyAttributeValue is failed: %@", kAXFocusedApplicationAttribute);
      result = NULL;
      goto finish;
    }
    baseElement = applicationElement;
  }

  error = AXUIElementCopyAttributeValue(baseElement,
                                        attribute,
                                        (CFTypeRef*)(&result));
  if (error != kAXErrorSuccess) {
    NSLog(@"AXUIElementCopyAttributeValue is failed: %@", attribute);
    result = NULL;
    goto finish;
  }

finish:
  // Do not release baseElement. (baseElement is not own.)

  if (applicationElement) {
    CFRelease(applicationElement);
    applicationElement = NULL;
  }
  if (systemWideElement) {
    CFRelease(systemWideElement);
    systemWideElement = NULL;
  }
  return result;
}

+ (AXUIElementRef) copyFocusedUIElement
{
  return [AXUtilities copyUIElement:kAXFocusedUIElementAttribute];
}

+ (AXUIElementRef) copyFocusedWindow
{
  return [AXUtilities copyUIElement:kAXFocusedWindowAttribute];
}

+ (AXUIElementRef) copyFrontmost
{
  return [AXUtilities copyUIElement:kAXFrontmostAttribute];
}

+ (AXUIElementRef) copyMainWindow
{
  return [AXUtilities copyUIElement:kAXMainWindowAttribute];
}

+ (NSArray*) attributeNamesOfUIElement:(AXUIElementRef)element
{
  if (! element) return nil;

  CFArrayRef attrNames = NULL;

  AXUIElementCopyAttributeNames(element, &attrNames);
  if (! attrNames) return nil;

  return (__bridge_transfer NSArray*)(attrNames);
}

+ (id) valueOfAttribute:(NSString*)attribute ofUIElement:(AXUIElementRef)element
{
  if (! attribute) return nil;
  if (! element) return nil;

  NSArray* attributeNames = [AXUtilities attributeNamesOfUIElement:element];

  if (! attributeNames) return nil;
  if ([attributeNames indexOfObject:attribute] == NSNotFound) return nil;

  CFTypeRef result = NULL;
  if (AXUIElementCopyAttributeValue(element, (__bridge CFStringRef)(attribute), &result) != kAXErrorSuccess) {
    return nil;
  }
  if (! result) return nil;

  return (__bridge_transfer id)(result);
}

+ (NSString*) titleOfUIElement:(AXUIElementRef)element
{
  if (! element) return nil;
  return (NSString*)[AXUtilities valueOfAttribute:NSAccessibilityTitleAttribute ofUIElement:element];
}

+ (NSString*) roleOfUIElement:(AXUIElementRef)element
{
  if (! element) return nil;
  return (NSString*)[AXUtilities valueOfAttribute:NSAccessibilityRoleAttribute ofUIElement:element];
}

@end
