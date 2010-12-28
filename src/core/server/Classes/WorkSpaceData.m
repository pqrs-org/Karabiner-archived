// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "WorkSpaceData.h"

@implementation WorkSpaceData

- (NSString*) getActiveApplicationName
{
  // ----------------------------------------
  NSWorkspace* ws = [NSWorkspace sharedWorkspace];
  if (! ws) return nil;

  NSArray* a = [ws runningApplications];
  if (! a) return nil;

  for (NSRunningApplication* app in a) {
    if (! app) return nil;

    if ([app isActive]) {
      NSString* nsappname = [app bundleIdentifier];

      if (nsappname) {
        return [NSString stringWithString:nsappname];

      } else {
        // We use localizedName instead of bundleIdentifier,
        // because "MacSOUP" doesn't have bundleIdentifier.
        // http://www.haller-berlin.de/macsoup/index.html
        NSString* localizedName = [app localizedName];
        if (localizedName) {
          return [NSString stringWithFormat:@"org.pqrs.unknownapp.%@", localizedName];
        }
      }
    }
  }

  return nil;
}

- (unsigned int) getApplicationType:(NSString*)name
{
  if (! name) return 0;

  if ([name isEqualToString:@"org.gnu.Emacs"] ||
      [name isEqualToString:@"org.gnu.AquamacsEmacs"] ||
      [name isEqualToString:@"org.gnu.Aquamacs"] ||
      [name isEqualToString:@"org.pqrs.unknownapp.conkeror"]) {
    return [configxmlparser_ keycode:@"ApplicationType::EMACS"];
  }

  if ([name hasPrefix:@"org.vim."]) {
    return [configxmlparser_ keycode:@"ApplicationType::VI"];
  }

  if ([name isEqualToString:@"com.apple.Terminal"]    ||
      [name isEqualToString:@"iTerm"]                 ||
      [name isEqualToString:@"net.sourceforge.iTerm"] ||
      [name isEqualToString:@"com.googlecode.iterm2"]) {
    return [configxmlparser_ keycode:@"ApplicationType::TERMINAL"];
  }

  if ([name isEqualToString:@"com.vmware.fusion"]             ||
      [name hasPrefix:@"com.vmware.proxyApp."]                || // unity
      [name isEqualToString:@"com.parallels.desktop"]         ||
      [name isEqualToString:@"com.parallels.vm"]              ||
      [name isEqualToString:@"com.parallels.desktop.console"] ||
      [name hasPrefix:@"com.parallels.winapp."]               || // coherence
      [name isEqualToString:@"org.virtualbox.app.VirtualBoxVM"]) {
    return [configxmlparser_ keycode:@"ApplicationType::VIRTUALMACHINE"];
  }

  if ([name isEqualToString:@"com.microsoft.rdc"] ||
      [name isEqualToString:@"net.sf.cord"]) {
    return [configxmlparser_ keycode:@"ApplicationType::REMOTEDESKTOPCONNECTION"];
  }

  if ([name isEqualToString:@"org.x.X11"] ||
      [name isEqualToString:@"com.apple.x11"] ||
      [name isEqualToString:@"org.macosforge.xquartz.X11"] ||
      [name isEqualToString:@"org.macports.X11"]) {
    return [configxmlparser_ keycode:@"ApplicationType::X11"];
  }

  if ([name isEqualToString:@"com.apple.finder"]) {
    return [configxmlparser_ keycode:@"ApplicationType::FINDER"];
  }

  if ([name isEqualToString:@"com.apple.Safari"]) {
    return [configxmlparser_ keycode:@"ApplicationType::SAFARI"];
  }

  if ([name isEqualToString:@"org.mozilla.firefox"]) {
    return [configxmlparser_ keycode:@"ApplicationType::FIREFOX"];
  }

  if ([name isEqualToString:@"org.mozilla.thunderbird"]) {
    return [configxmlparser_ keycode:@"ApplicationType::THUNDERBIRD"];
  }

  if ([name isEqualToString:@"com.apple.iChat"]) {
    return [configxmlparser_ keycode:@"ApplicationType::ICHAT"];
  }

  if ([name isEqualToString:@"com.adiumX.adiumX"]) {
    return [configxmlparser_ keycode:@"ApplicationType::ADIUMX"];
  }

  if ([name isEqualToString:@"com.skype.skype"]) {
    return [configxmlparser_ keycode:@"ApplicationType::SKYPE"];
  }

  if ([name isEqualToString:@"com.apple.mail"]) {
    return [configxmlparser_ keycode:@"ApplicationType::MAIL"];
  }

  if ([name isEqualToString:@"com.apple.TextEdit"]) {
    return [configxmlparser_ keycode:@"ApplicationType::EDITOR"];
  }

  if ([name isEqualToString:@"com.adobe.Photoshop"] ||
      [name isEqualToString:@"com.adobe.Photoshop.Elements"]) {
    return [configxmlparser_ keycode:@"ApplicationType::PHOTOSHOP"];
  }

  if ([name isEqualToString:@"com.adobe.illustrator"]) {
    return [configxmlparser_ keycode:@"ApplicationType::ILLUSTRATOR"];
  }

  if ([name hasPrefix:@"com.adobe."]) {
    return [configxmlparser_ keycode:@"ApplicationType::ADOBE"];
  }

  if ([name isEqualToString:@"com.microsoft.Excel"]) {
    return [configxmlparser_ keycode:@"ApplicationType::EXCEL"];
  }

  if ([name isEqualToString:@"com.microsoft.Powerpoint"]) {
    return [configxmlparser_ keycode:@"ApplicationType::POWERPOINT"];
  }

  if ([name isEqualToString:@"com.microsoft.Word"]) {
    return [configxmlparser_ keycode:@"ApplicationType::WORD"];
  }

  if ([name isEqualToString:@"com.microsoft.Outlook"]) {
    return [configxmlparser_ keycode:@"ApplicationType::OUTLOOK"];
  }

  if ([name isEqualToString:@"com.microsoft.Entourage"]) {
    return [configxmlparser_ keycode:@"ApplicationType::ENTOURAGE"];
  }

  if ([name isEqualToString:@"org.eclipse.eclipse"] ||
      [name isEqualToString:@"com.springsource.sts"]) {
    return [configxmlparser_ keycode:@"ApplicationType::ECLIPSE"];
  }

  if ([name isEqualToString:@"org.pqrs.unknownapp.MacSOUP"]) {
    return [configxmlparser_ keycode:@"ApplicationType::MACSOUP"];
  }

  if ([name isEqualToString:@"uk.co.opencommunity.vienna2"]) {
    return [configxmlparser_ keycode:@"ApplicationType::VIENNA"];
  }

  if ([name isEqualToString:@"com.apple.Xcode"]) {
    return [configxmlparser_ keycode:@"ApplicationType::XCODE"];
  }

  if ([name isEqualToString:@"com.operasoftware.Opera"]) {
    return [configxmlparser_ keycode:@"ApplicationType::OPERA"];
  }

  if ([name isEqualToString:@"com.google.Chrome"]) {
    return [configxmlparser_ keycode:@"ApplicationType::GOOGLE_CHROME"];
  }

  if ([name isEqualToString:@"com.TeamViewer.TeamViewer"]) {
    return [configxmlparser_ keycode:@"ApplicationType::TEAMVIEWER"];
  }

  if ([name isEqualToString:@"de.wengenmayer.Cheetah3D"]) {
    return [configxmlparser_ keycode:@"ApplicationType::CHEETAH_3D"];
  }

  if ([name isEqualToString:@"com.apple.securityagent"]) {
    return [configxmlparser_ keycode:@"ApplicationType::PASSWORD"];
  }

  if ([name isEqualToString:@"com.sakya.Gruml"]) {
    return [configxmlparser_ keycode:@"ApplicationType::GRUML"];
  }

  if ([name isEqualToString:@"com.apple.iPhoto"]) {
    return [configxmlparser_ keycode:@"ApplicationType::IPHOTO"];
  }

  if ([name isEqualToString:@"com.apple.Stickies"]) {
    return [configxmlparser_ keycode:@"ApplicationType::STICKIES"];
  }

  if ([name isEqualToString:@"com.jetbrains.webide"]) {
    return [configxmlparser_ keycode:@"ApplicationType::WEBIDE"];
  }

  if ([name isEqualToString:@"at.obdev.LaunchBar"]) {
    return [configxmlparser_ keycode:@"ApplicationType::LAUNCHBAR"];
  }

  if ([name isEqualToString:@"com.apple.ScreenSharing"]) {
    return [configxmlparser_ keycode:@"ApplicationType::SCREEN_SHARING"];
  }

  if ([name isEqualToString:@"com.geekspiff.chickenofthevnc"]) {
    return [configxmlparser_ keycode:@"ApplicationType::VNC"];
  }

  if ([name isEqualToString:@"com.apple.iWork.Pages"]) {
    return [configxmlparser_ keycode:@"ApplicationType::IWORK_PAGES"];
  }

  if ([name isEqualToString:@"com.apple.iWork.Numbers"]) {
    return [configxmlparser_ keycode:@"ApplicationType::IWORK_NUMBERS"];
  }

  if ([name isEqualToString:@"com.apple.iWork.Keynote"]) {
    return [configxmlparser_ keycode:@"ApplicationType::IWORK_KEYNOTE"];
  }

  if ([name isEqualToString:@"com.luxology.modo"]) {
    return [configxmlparser_ keycode:@"ApplicationType::MODO"];
  }

  if ([name isEqualToString:@"com.blizzard.worldofwarcraft"]) {
    return [configxmlparser_ keycode:@"ApplicationType::WORLD_OF_WARCRAFT"];
  }

  return [configxmlparser_ keycode:@"ApplicationType::UNKNOWN"];
}

@end
