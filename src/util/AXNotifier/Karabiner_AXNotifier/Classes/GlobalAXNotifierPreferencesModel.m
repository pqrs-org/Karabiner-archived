#import "GlobalAXNotifierPreferencesModel.h"
#import "PreferencesModel.h"

static AXNotifierPreferencesModel* axNotifierPreferencesModel_ = nil;

@implementation GlobalAXNotifierPreferencesModel

+ (AXNotifierPreferencesModel*)get {
  return axNotifierPreferencesModel_;
}

+ (void)set:(AXNotifierPreferencesModel*)newvalue {
  axNotifierPreferencesModel_ = newvalue;
}

+ (BOOL)debuggingLogEnabled {
  if (!axNotifierPreferencesModel_) {
    return NO;
  }
  return axNotifierPreferencesModel_.debuggingLogEnabled;
}

@end
