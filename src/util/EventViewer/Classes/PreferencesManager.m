#import "PreferencesManager.h"
#import "PreferencesKeys.h"

@implementation PreferencesManager

+ (void)initialize {
  NSDictionary* dict = @{
    kForceStayTop : @NO,
    kShowInAllSpaces : @NO,
    kHideIgnorableEvents : @YES,
  };
  [[NSUserDefaults standardUserDefaults] registerDefaults:dict];
}

@end
