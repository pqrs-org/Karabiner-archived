#import "PreferencesKeys.h"
#import "PreferencesManager.h"

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
