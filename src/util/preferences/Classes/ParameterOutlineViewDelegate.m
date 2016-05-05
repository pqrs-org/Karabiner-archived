#import "ParameterOutlineViewDelegate.h"
#import "ParameterDiffCellView.h"
#import "ParameterTree.h"
#import "ParameterValueCellView.h"
#import "PreferencesWindowController.h"
#import "ServerClient.h"

@interface ParameterOutlineViewDelegate ()

@property(weak) IBOutlet PreferencesModel* preferencesModel;
@property(weak) IBOutlet PreferencesWindowController* preferencesWindowController;
@property(weak) IBOutlet ServerClient* client;

@end

@implementation ParameterOutlineViewDelegate

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  ParameterTree* tree = (ParameterTree*)(item);
  if (!tree || !tree.node) return nil;

  if ([tableColumn.identifier isEqualToString:@"ParameterNameColumn"]) {
    NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterNameCellView" owner:self];
    result.textField.stringValue = tree.node.name;
    return result;

  } else {
    if ([tree.node.identifier length] == 0) {
      return nil;
    }

    if ([tableColumn.identifier isEqualToString:@"ParameterDefaultValueColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDefaultValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%ld", tree.node.defaultValue];
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterValueColumn"]) {
      int value = [self.client.proxy value:tree.node.identifier];
      ParameterValueCellView* result = [outlineView makeViewWithIdentifier:@"ParameterValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%d", value];
      result.stepper.integerValue = value;
      result.stepper.minValue = 0;
      result.stepper.maxValue = 1073741824; // 2^30
      result.stepper.increment = tree.node.step;
      result.stepper.autorepeat = YES;
      result.stepper.valueWraps = NO;
      result.preferencesModel = self.preferencesModel;
      result.preferencesWindowController = self.preferencesWindowController;
      result.client = self.client;
      result.settingIdentifier = tree.node.identifier;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterBaseUnitColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterBaseUnitCellView" owner:self];
      result.textField.stringValue = tree.node.baseUnit;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterDiffColumn"]) {
      ParameterDiffCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDiffCellView" owner:self];
      result.preferencesModel = self.preferencesModel;
      result.preferencesWindowController = self.preferencesWindowController;
      result.client = self.client;
      result.settingIdentifier = tree.node.identifier;
      result.defaultValue = tree.node.defaultValue;
      [result setObserver];
      [result updateValue];
      return result;
    }
  }

  return nil;
}

@end
