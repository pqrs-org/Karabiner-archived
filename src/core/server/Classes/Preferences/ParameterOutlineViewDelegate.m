#import "ParameterOutlineViewDelegate.h"

@implementation ParameterOutlineViewDelegate

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  if ([tableColumn.identifier isEqualToString:@"ParameterNameColumnView"]) {
    NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterNameCellView" owner:self];
    NSString* value = item[@"name"];
    if (value) {
      result.textField.stringValue = value;
    }
    return result;

  } else {
    NSString* identifier = item[@"identifier"];

    if (!identifier) {
      return nil;
    }

    if ([tableColumn.identifier isEqualToString:@"ParameterDefaultValueColumnView"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDefaultValueCellView" owner:self];
      NSString* value = item[@"default"];
      if (value) {
        result.textField.stringValue = value;
      }
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterValueColumnView"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterValueCellView" owner:self];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterBaseUnitColumnView"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterBaseUnitCellView" owner:self];
      NSString* value = item[@"baseunit"];
      if (value) {
        result.textField.stringValue = value;
      }
      return result;
    }
  }

  return nil;
}

@end
