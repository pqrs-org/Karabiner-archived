#import "AppLauncher.h"
#import "XMLCompiler.h"

@implementation AppLauncher

+ (void)openPreferences {
  NSString* path = @"/Applications/Karabiner.app/Contents/Applications/Karabiner Preferences.app";
  [[NSWorkspace sharedWorkspace] launchApplication:path];
}

+ (void)openEventViewer {
  NSString* path = @"/Applications/Karabiner.app/Contents/Applications/EventViewer.app";
  [[NSWorkspace sharedWorkspace] launchApplication:path];
}

+ (void)openMultiTouchExtension {
  [[NSWorkspace sharedWorkspace] launchApplication:@"/Applications/Karabiner.app/Contents/Applications/Karabiner_multitouchextension.app"];
}

+ (void)openPrivateXMLDirectory {
  NSString* path = [XMLCompiler get_private_xml_path];
  if ([path length] > 0) {
    [[NSWorkspace sharedWorkspace] openFile:[path stringByDeletingLastPathComponent]];
  }
}

+ (void)openSystemPreferencesKeyboard {
  [[NSWorkspace sharedWorkspace] openFile:@"/System/Library/PreferencePanes/Keyboard.prefPane"];
}

+ (void)openUninstaller {
  NSString* path = @"/Library/Application Support/org.pqrs/Karabiner/uninstaller.applescript";
  [[[NSAppleScript alloc] initWithContentsOfURL:[NSURL fileURLWithPath:path] error:nil] executeAndReturnError:nil];
}

@end
