#import "CheckboxCellView.h"
#import "PreferencesManager.h"

@interface CheckboxCellView()
@property(weak) IBOutlet PreferencesManager* preferencesManager;

@end

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
  NSLog(@"valueChanged: %@", self.settingIdentifier);
}

@end
