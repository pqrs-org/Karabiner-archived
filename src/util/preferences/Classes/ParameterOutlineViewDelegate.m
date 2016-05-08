#import "ParameterOutlineViewDelegate.h"
#import "ParameterDiffCellView.h"
#import "ParameterTree.h"
#import "ParameterValueCellView.h"
#import "PreferencesModel.h"
#import "PreferencesWindowController.h"
#import "ServerClient.h"
#import "SharedPreferencesManager.h"

@interface ParameterOutlineViewDelegate ()

@property(weak) IBOutlet SharedPreferencesManager* sharedPreferencesManager;

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
      NSInteger value = [self.sharedPreferencesManager.pm value:tree.node.identifier];
      ParameterValueCellView* result = [outlineView makeViewWithIdentifier:@"ParameterValueCellView" owner:self];
      result.textField.stringValue = [NSString stringWithFormat:@"%d", (int)(value)];
      result.stepper.integerValue = value;
      result.stepper.minValue = 0;
      result.stepper.maxValue = 1073741824; // 2^30
      result.stepper.increment = tree.node.step;
      result.stepper.autorepeat = YES;
      result.stepper.valueWraps = NO;
      result.sharedPreferencesManager = self.sharedPreferencesManager;
      result.settingIdentifier = tree.node.identifier;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterBaseUnitColumn"]) {
      NSTableCellView* result = [outlineView makeViewWithIdentifier:@"ParameterBaseUnitCellView" owner:self];
      result.textField.stringValue = tree.node.baseUnit;
      return result;

    } else if ([tableColumn.identifier isEqualToString:@"ParameterDiffColumn"]) {
      ParameterDiffCellView* result = [outlineView makeViewWithIdentifier:@"ParameterDiffCellView" owner:self];
      result.sharedPreferencesManager = self.sharedPreferencesManager;
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
