#import "ParameterOutlineViewDelegate.h"
#import "ParameterDiffCellView.h"
#import "ParameterValueCellView.h"
#import "PreferencesWindowController.h"
#import "ServerClient.h"
#import "SharedXMLCompilerTree.h"

@interface ParameterOutlineViewDelegate ()

@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property(weak) IBOutlet ServerClient* client;

@end

@implementation ParameterOutlineViewDelegate

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  SharedXMLCompilerTree* tree = (SharedXMLCompilerTree*)(item);

  if ([tableColumn.identifier isEqualToString:@"ParameterNameColumn"]) {
    NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterNameCellView" owner:self];
    result.textField.stringValue = [self.client.proxy parameterItemGetName:tree.id];
    return result;

  } else {
    NSString* identifier = [self.client.proxy parameterItemGetIdentifier:tree.id];
    if ([identifier length] == 0) {
      return nil;
    }

    if ([tableColumn.identifier isEqualToString:@"ParameterDefaultValueColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDefaultValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%ld", [self.client.proxy parameterItemGetDefaultValue:tree.id]];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterValueColumn"]) {
      int value = [self.client.proxy value:identifier];
      ParameterValueCellView* result = [outlineView makeViewWithIdentifier:@"ParameterValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%d", value];
      result.stepper.integerValue = value;
      result.stepper.minValue = 0;
      result.stepper.maxValue = 1073741824; // 2^30
      result.stepper.increment = [self.client.proxy parameterItemGetStep:tree.id];
      result.stepper.autorepeat = YES;
      result.stepper.valueWraps = NO;
      result.preferencesModel = self.preferencesModel;
      result.preferencesWindowController = self.preferencesWindowController;
      result.settingIdentifier = identifier;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterBaseUnitColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterBaseUnitCellView" owner:self];
      result.textField.stringValue = [self.client.proxy parameterItemGetBaseUnit:tree.id];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterDiffColumn"]) {
      ParameterDiffCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDiffCellView" owner:self];
      result.preferencesModel = self.preferencesModel;
      result.preferencesWindowController = self.preferencesWindowController;
      result.settingIdentifier = identifier;
      result.defaultValue = [self.client.proxy parameterItemGetDefaultValue:tree.id];
      [result setObserver];
      [result updateValue];
      return result;
    }
  }

  return nil;
}

@end
