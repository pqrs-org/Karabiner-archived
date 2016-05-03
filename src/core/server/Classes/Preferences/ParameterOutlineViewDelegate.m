#import "ParameterOutlineViewDelegate.h"
#import "ParameterDiffCellView.h"
#import "ParameterValueCellView.h"
#import "PreferencesWindowController.h"
#import "ServerForUserspace.h"
#import "ServerObjects.h"
#import "SharedXMLCompilerTree.h"
#import "XMLCompiler.h"

@interface ParameterOutlineViewDelegate ()

@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;

@end

@implementation ParameterOutlineViewDelegate

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);

  if ([tableColumn.identifier isEqualToString:@"ParameterNameColumn"]) {
    NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterNameCellView" owner:self];
    result.textField.stringValue = [self.preferencesWindowController.serverObjects.serverForUserspace parameterItemGetName:tree.id];
    return result;

  } else {
    NSString* identifier = [self.preferencesWindowController.serverObjects.serverForUserspace parameterItemGetIdentifier:tree.id];
    if ([identifier length] == 0) {
      return nil;
    }

    if ([tableColumn.identifier isEqualToString:@"ParameterDefaultValueColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDefaultValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%ld", [self.preferencesWindowController.serverObjects.serverForUserspace parameterItemGetDefaultValue:tree.id]];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterValueColumn"]) {
      int value = [self.preferencesWindowController.serverObjects.serverForUserspace value:identifier];
      ParameterValueCellView* result = [outlineView makeViewWithIdentifier:@"ParameterValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%d", value];
      result.stepper.integerValue = value;
      result.stepper.minValue = 0;
      result.stepper.maxValue = 1073741824; // 2^30
      result.stepper.increment = [self.preferencesWindowController.serverObjects.serverForUserspace parameterItemGetStep:tree.id];
      result.stepper.autorepeat = YES;
      result.stepper.valueWraps = NO;
      result.serverObjects = self.preferencesWindowController.serverObjects;
      result.settingIdentifier = identifier;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterBaseUnitColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterBaseUnitCellView" owner:self];
      result.textField.stringValue = [self.preferencesWindowController.serverObjects.serverForUserspace parameterItemGetBaseUnit:tree.id];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterDiffColumn"]) {
      ParameterDiffCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDiffCellView" owner:self];
      result.serverObjects = self.preferencesWindowController.serverObjects;
      result.settingIdentifier = identifier;
      result.defaultValue = [self.preferencesWindowController.serverObjects.serverForUserspace parameterItemGetDefaultValue:tree.id];
      [result setObserver];
      [result updateValue];
      return result;
    }
  }

  return nil;
}

@end
