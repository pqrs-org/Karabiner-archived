#import "ParameterOutlineViewDelegate.h"
#import "ParameterValueCellView.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@implementation ParameterOutlineViewDelegate

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  ParameterItem* parameterItem = [tree castNodeToParameterItem];

  if ([tableColumn.identifier isEqualToString:@"ParameterNameColumn"]) {
    NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterNameCellView" owner:self];
    result.textField.stringValue = [parameterItem getName];
    return result;

  } else {
    NSString* identifier = [parameterItem getIdentifier];
    if ([identifier length] == 0) {
      return nil;
    }

    if ([tableColumn.identifier isEqualToString:@"ParameterDefaultValueColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDefaultValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%ld", [parameterItem getDefaultValue]];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterValueColumn"]) {
      int value = [self.preferencesManager value:identifier];
      ParameterValueCellView* result = [outlineView makeViewWithIdentifier:@"ParameterValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%d", value];
      result.stepper.integerValue = value;
      result.stepper.minValue = 0;
      result.stepper.maxValue = 1073741824; // 2^30
      result.stepper.increment = [parameterItem getStep];
      // autorepeat has bug.
      // The repeat receiver will be changed randomly.
      // Therefore, we have to disable autorepeat.
      result.stepper.autorepeat = NO;
      result.stepper.valueWraps = NO;
      result.preferencesManager = self.preferencesManager;
      result.settingIdentifier = identifier;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterBaseUnitColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterBaseUnitCellView" owner:self];
      result.textField.stringValue = [parameterItem getBaseUnit];
      return result;
    }
  }

  return nil;
}

@end
