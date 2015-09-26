#import "EnvironmentChecker.h"

@implementation EnvironmentChecker

+ (BOOL)checkDoubleCommand {
  @synchronized(self) {
    static dispatch_once_t onceToken = 0;
    static BOOL result = NO;

    dispatch_once(&onceToken, ^{
      // Check file system.
      // (Do not determine by kextstat because loading kext might be lazy loading.)
      NSArray* paths = @[ @"/Library/StartupItems/DoubleCommand" ];
      for (NSString* path in paths) {
        if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
          result = YES;
          break;
        }
      }
    });

    return result;
  }
}

+ (BOOL)checkKeyRemap4MacBook {
  @synchronized(self) {
    static dispatch_once_t onceToken = 0;
    static BOOL result = NO;

    dispatch_once(&onceToken, ^{
      // Check kextstat.
      //
      // KeyRemap4MacBook and Karabiner are running together only when upgrading
      // (KeyRemap4MacBook -> Karabiner) and failed to unload kext in installer.
      //
      // So, KeyRemap4MacBook kext has been removed from file system by installer.
      // We have to check kext by kextstat.

      if (system("/bin/test -n \"`/usr/sbin/kextstat -l -b org.pqrs.driver.KeyRemap4MacBook`\"") == 0) {
        result = YES;
      }
    });

    return result;
  }
}

+ (BOOL)checkKirgudu {
  @synchronized(self) {
    static dispatch_once_t onceToken = 0;
    static BOOL result = NO;

    dispatch_once(&onceToken, ^{
      // Check kextstat.
      if (system("/bin/test -n \"`/usr/sbin/kextstat -l -b com.pihto.driver.KirguduDriver`\"") == 0) {
        result = YES;
      }
    });

    return result;
  }
}

+ (BOOL)checkSmoothMouse {
  @synchronized(self) {
    static dispatch_once_t onceToken = 0;
    static BOOL result = NO;

    dispatch_once(&onceToken, ^{
      // Check file system.
      // (Do not determine by kextstat because loading kext might be lazy loading.)
      NSArray* paths = @[ @"/System/Library/Extensions/SmoothMouse.kext",
                          @"/Library/PreferencePanes/SmoothMouse.prefPane" ];
      for (NSString* path in paths) {
        if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
          result = YES;
          break;
        }
      }
    });

    return result;
  }
}

@end
