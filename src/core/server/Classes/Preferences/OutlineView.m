#import "NotificationKeys.h"
#import "OutlineView.h"
#import "ParameterOutlineViewDataSource.h"
#import "PreferencesManager.h"
#import "XMLCompiler.h"

@interface OutlineView ()
@property(weak) IBOutlet ParameterOutlineViewDataSource* parameterOutlineViewDataSource;
@end

@implementation OutlineView

+ (NSFont*)font {
  return [NSFont systemFontOfSize:[NSFont smallSystemFontSize]];
}

- (NSCell*)outlineView:(NSOutlineView*)outlineView dataCellForTableColumn:(NSTableColumn*)tableColumn item:(id)item {
  if (!tableColumn) return nil;

  if (ischeckbox_) {
    return nil;
  } else {
    NSString* identifier = item[@"identifier"];
    NSString* columnIdentifier = [tableColumn identifier];

    if (identifier) {
      if ([columnIdentifier isEqualToString:@"value"]) {
        NSTextFieldCell* cell = [NSTextFieldCell new];
        [cell setFont:[OutlineView font]];
        [cell setEditable:YES];
        return cell;

      } else if ([columnIdentifier isEqualToString:@"stepper"]) {
        NSStepperCell* cell = [NSStepperCell new];
        [cell setFont:[OutlineView font]];

        [cell setMinValue:0];
        [cell setMaxValue:1073741824]; // 2^30
        [cell setIncrement:[item[@"step"] intValue]];
        [cell setAutorepeat:NO];
        [cell setValueWraps:NO];
        return cell;
      }
    }

    // ----------------------------------------
    NSCell* cell = [NSCell new];
    [cell setFont:[OutlineView font]];

    if ([columnIdentifier isEqualToString:@"default"]) {
      [cell setEnabled:NO];
    }

    return cell;
  }
}

@end
