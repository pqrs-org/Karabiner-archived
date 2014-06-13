#import "AXUtilities.h"

@implementation AXUtilities

+ (AXUIElementRef) copyFocusedUIElement
{
  if (! AXIsProcessTrusted()) return NULL;

  AXUIElementRef result = NULL;
  AXUIElementRef systemWideElement = NULL;
  AXError error = kAXErrorSuccess;

  systemWideElement = AXUIElementCreateSystemWide();
  if (! systemWideElement) goto finish;

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

+ (AXUIElementRef) copyFocusedWindow:(AXUIElementRef)applicationElement
{
  AXUIElementRef result = NULL;
  AXError error = AXUIElementCopyAttributeValue(applicationElement,
                                                kAXFocusedWindowAttribute,
                                                (CFTypeRef*)(&result));
  if (error != kAXErrorSuccess) {
    NSLog(@"copyFocusedWindow is failed. error:%d", error);
    return NULL;
  }
  return result;
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

+ (NSString*) subroleOfUIElement:(AXUIElementRef)element
{
  if (! element) return nil;
  return (NSString*)[AXUtilities valueOfAttribute:NSAccessibilitySubroleAttribute ofUIElement:element];
}

@end
