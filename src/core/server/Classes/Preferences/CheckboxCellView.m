#import "CheckboxCellView.h"
#import "PreferencesManager.h"

@implementation CheckboxCellView

- (void)toggleCheckboxState {
  if (self.checkbox.enabled) {
    if (self.checkbox.state == NSOnState) {
      self.checkbox.state = NSOffState;
    } else {
      self.checkbox.state = NSOnState;
    }
    [self valueChanged:self];
  }
}

- (IBAction)valueChanged:(id)sender {
  if (self.checkbox.enabled) {
    int value = (self.checkbox.state == NSOnState);
    [self.preferencesManager setValue:value forName:self.settingIdentifier];
  }
}

@end
