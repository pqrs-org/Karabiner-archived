#import "ParameterDiffCellView.h"
#import "ParameterOutlineViewDelegate.h"
#import "ParameterValueCellView.h"
#import "PreferencesManager.h"
#import "PreferencesWindowController.h"
#import "ServerObjects.h"
#import "XMLCompiler.h"

@interface ParameterOutlineViewDelegate ()

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;

@end

@implementation ParameterOutlineViewDelegate

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  XMLCompilerTree* tree = (XMLCompilerTree*)(item);
  ParameterItem* parameterItem = [tree.node castToParameterItem];

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
      int value = [self.preferencesWindowController.serverObjects.preferencesManager value:identifier];
      ParameterValueCellView* result = [outlineView makeViewWithIdentifier:@"ParameterValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%d", value];
      result.stepper.integerValue = value;
      result.stepper.minValue = 0;
      result.stepper.maxValue = 1073741824; // 2^30
      result.stepper.increment = [parameterItem getStep];
      result.stepper.autorepeat = YES;
      result.stepper.valueWraps = NO;
      result.serverObjects = self.preferencesWindowController.serverObjects;
      result.settingIdentifier = identifier;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterBaseUnitColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterBaseUnitCellView" owner:self];
      result.textField.stringValue = [parameterItem getBaseUnit];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterDiffColumn"]) {
      ParameterDiffCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDiffCellView" owner:self];
      result.serverObjects = self.preferencesWindowController.serverObjects;
      result.settingIdentifier = identifier;
      result.defaultValue = [parameterItem getDefaultValue];
      [result setObserver];
      [result updateValue];
      return result;
    }
  }

  return nil;
}

@end
