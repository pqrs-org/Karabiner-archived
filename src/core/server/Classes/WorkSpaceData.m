// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "WorkSpaceData.h"

@implementation WorkSpaceData

+ (NSString*) getActiveApplicationName
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

+ (unsigned int) getApplicationType:(NSString*)name
{
  if (! name) return 0;

  ConfigXMLParser* parser = [ConfigXMLParser getInstance];

  if ([name isEqualToString:@"org.gnu.Emacs"] ||
      [name isEqualToString:@"org.gnu.AquamacsEmacs"] ||
      [name isEqualToString:@"org.gnu.Aquamacs"] ||
      [name isEqualToString:@"org.pqrs.unknownapp.conkeror"]) {
    return [parser keycode:@"ApplicationType::EMACS"];
  }

  if ([name hasPrefix:@"org.vim."]) {
    return [parser keycode:@"ApplicationType::VI"];
  }

  if ([name hasPrefix:@"com.macromates.textmate"]) {
    return [parser keycode:@"ApplicationType::TEXTMATE"];
  }

  if ([name isEqualToString:@"com.apple.Terminal"]    ||
      [name isEqualToString:@"iTerm"]                 ||
      [name isEqualToString:@"net.sourceforge.iTerm"] ||
      [name isEqualToString:@"com.googlecode.iterm2"]) {
    return [parser keycode:@"ApplicationType::TERMINAL"];
  }

  if ([name isEqualToString:@"com.vmware.fusion"]             ||
      [name hasPrefix:@"com.vmware.proxyApp."]                || // unity
      [name isEqualToString:@"com.parallels.desktop"]         ||
      [name isEqualToString:@"com.parallels.vm"]              ||
      [name isEqualToString:@"com.parallels.desktop.console"] ||
      [name hasPrefix:@"com.parallels.winapp."]               || // coherence
      [name isEqualToString:@"org.virtualbox.app.VirtualBoxVM"]) {
    return [parser keycode:@"ApplicationType::VIRTUALMACHINE"];
  }

  if ([name isEqualToString:@"com.microsoft.rdc"] ||
      [name isEqualToString:@"net.sf.cord"]) {
    return [parser keycode:@"ApplicationType::REMOTEDESKTOPCONNECTION"];
  }

  if ([name isEqualToString:@"org.x.X11"] ||
      [name isEqualToString:@"com.apple.x11"] ||
      [name isEqualToString:@"org.macosforge.xquartz.X11"] ||
      [name isEqualToString:@"org.macports.X11"]) {
    return [parser keycode:@"ApplicationType::X11"];
  }

  if ([name isEqualToString:@"com.apple.finder"]) {
    return [parser keycode:@"ApplicationType::FINDER"];
  }

  if ([name isEqualToString:@"com.apple.Safari"]) {
    return [parser keycode:@"ApplicationType::SAFARI"];
  }

  if ([name isEqualToString:@"org.mozilla.firefox"]) {
    return [parser keycode:@"ApplicationType::FIREFOX"];
  }

  if ([name isEqualToString:@"org.mozilla.thunderbird"]) {
    return [parser keycode:@"ApplicationType::THUNDERBIRD"];
  }

  if ([name isEqualToString:@"com.apple.iChat"]) {
    return [parser keycode:@"ApplicationType::ICHAT"];
  }

  if ([name isEqualToString:@"com.adiumX.adiumX"]) {
    return [parser keycode:@"ApplicationType::ADIUMX"];
  }

  if ([name isEqualToString:@"com.skype.skype"]) {
    return [parser keycode:@"ApplicationType::SKYPE"];
  }

  if ([name isEqualToString:@"com.apple.mail"]) {
    return [parser keycode:@"ApplicationType::MAIL"];
  }

  if ([name isEqualToString:@"com.apple.TextEdit"]) {
    return [parser keycode:@"ApplicationType::EDITOR"];
  }

  if ([name isEqualToString:@"com.adobe.Photoshop"] ||
      [name isEqualToString:@"com.adobe.Photoshop.Elements"]) {
    return [parser keycode:@"ApplicationType::PHOTOSHOP"];
  }

  if ([name isEqualToString:@"com.adobe.illustrator"]) {
    return [parser keycode:@"ApplicationType::ILLUSTRATOR"];
  }

  if ([name hasPrefix:@"com.adobe."]) {
    return [parser keycode:@"ApplicationType::ADOBE"];
  }

  if ([name isEqualToString:@"com.microsoft.Excel"]) {
    return [parser keycode:@"ApplicationType::EXCEL"];
  }

  if ([name isEqualToString:@"com.microsoft.Powerpoint"]) {
    return [parser keycode:@"ApplicationType::POWERPOINT"];
  }

  if ([name isEqualToString:@"com.microsoft.Word"]) {
    return [parser keycode:@"ApplicationType::WORD"];
  }

  if ([name isEqualToString:@"com.microsoft.Outlook"]) {
    return [parser keycode:@"ApplicationType::OUTLOOK"];
  }

  if ([name isEqualToString:@"com.microsoft.Entourage"]) {
    return [parser keycode:@"ApplicationType::ENTOURAGE"];
  }

  if ([name isEqualToString:@"org.eclipse.eclipse"] ||
      [name isEqualToString:@"com.springsource.sts"]) {
    return [parser keycode:@"ApplicationType::ECLIPSE"];
  }

  if ([name isEqualToString:@"org.pqrs.unknownapp.MacSOUP"]) {
    return [parser keycode:@"ApplicationType::MACSOUP"];
  }

  if ([name isEqualToString:@"uk.co.opencommunity.vienna2"]) {
    return [parser keycode:@"ApplicationType::VIENNA"];
  }

  if ([name isEqualToString:@"com.apple.Xcode"]) {
    return [parser keycode:@"ApplicationType::XCODE"];
  }

  if ([name isEqualToString:@"com.operasoftware.Opera"] ||
      [name isEqualToString:@"com.operasoftware.OperaNext"]) {
    return [parser keycode:@"ApplicationType::OPERA"];
  }

  if ([name isEqualToString:@"com.google.Chrome"]) {
    return [parser keycode:@"ApplicationType::GOOGLE_CHROME"];
  }

  if ([name isEqualToString:@"com.TeamViewer.TeamViewer"]) {
    return [parser keycode:@"ApplicationType::TEAMVIEWER"];
  }

  if ([name isEqualToString:@"de.wengenmayer.Cheetah3D"]) {
    return [parser keycode:@"ApplicationType::CHEETAH_3D"];
  }

  if ([name isEqualToString:@"com.apple.securityagent"]) {
    return [parser keycode:@"ApplicationType::PASSWORD"];
  }

  if ([name isEqualToString:@"com.sakya.Gruml"]) {
    return [parser keycode:@"ApplicationType::GRUML"];
  }

  if ([name isEqualToString:@"com.apple.iPhoto"]) {
    return [parser keycode:@"ApplicationType::IPHOTO"];
  }

  if ([name isEqualToString:@"com.apple.Stickies"]) {
    return [parser keycode:@"ApplicationType::STICKIES"];
  }

  if ([name isEqualToString:@"com.jetbrains.webide"]) {
    return [parser keycode:@"ApplicationType::WEBIDE"];
  }

  if ([name isEqualToString:@"at.obdev.LaunchBar"]) {
    return [parser keycode:@"ApplicationType::LAUNCHBAR"];
  }

  if ([name isEqualToString:@"com.apple.ScreenSharing"]) {
    return [parser keycode:@"ApplicationType::SCREEN_SHARING"];
  }

  // JollysFastVNC Home http://www.jinx.de/JollysFastVNC.html
  // de.jinx.JollysFastVNC, de.jinx.JollysFastVNC.Home, ...
  if ([name isEqualToString:@"com.geekspiff.chickenofthevnc"] ||
      [name hasPrefix:@"de.jinx.JollysFastVNC"]) {
    return [parser keycode:@"ApplicationType::VNC"];
  }

  if ([name isEqualToString:@"com.apple.iWork.Pages"]) {
    return [parser keycode:@"ApplicationType::IWORK_PAGES"];
  }

  if ([name isEqualToString:@"com.apple.iWork.Numbers"]) {
    return [parser keycode:@"ApplicationType::IWORK_NUMBERS"];
  }

  if ([name isEqualToString:@"com.apple.iWork.Keynote"]) {
    return [parser keycode:@"ApplicationType::IWORK_KEYNOTE"];
  }

  if ([name isEqualToString:@"com.luxology.modo"]) {
    return [parser keycode:@"ApplicationType::MODO"];
  }

  if ([name isEqualToString:@"com.blizzard.worldofwarcraft"]) {
    return [parser keycode:@"ApplicationType::WORLD_OF_WARCRAFT"];
  }

  if ([name isEqualToString:@"com.blizzard.starcraft2"]) {
    return [parser keycode:@"ApplicationType::STARCRAFT2"];
  }

  // com.ctmdev.FoxTrot
  // com.ctmdev.FoxTrotPro
  if ([name hasPrefix:@"com.ctmdev.FoxTrot"]) {
    return [parser keycode:@"ApplicationType::FOXTROT"];
  }

  // com.binarynights.ForkLift
  // com.binarynights.ForkLift2
  if ([name hasPrefix:@"com.binarynights.ForkLift"]) {
    return [parser keycode:@"ApplicationType::FORKLIFT"];
  }

  if ([name isEqualToString:@"net.limechat.LimeChat"]) {
    return [parser keycode:@"ApplicationType::LIMECHAT"];
  }

  // com.newtek.LightWave.Modeler
  // com.newtek.LightWave.Layout
  // com.newtek.LightWave.Hub
  if ([name hasPrefix:@"com.newtek.LightWave"]) {
    return [parser keycode:@"ApplicationType::LIGHTWAVE"];
  }

  if ([name isEqualToString:@"org.libreoffice.script"]) {
    return [parser keycode:@"ApplicationType::LIBREOFFICE"];
  }

  if ([name isEqualToString:@"org.videolan.vlc"]) {
    return [parser keycode:@"ApplicationType::VLC"];
  }

  if ([name isEqualToString:@"com.apple.iTunes"]) {
    return [parser keycode:@"ApplicationType::ITUNES"];
  }

  if ([name isEqualToString:@"com.apple.Dictionary"]) {
    return [parser keycode:@"ApplicationType::DICTIONARY"];
  }

  if ([name isEqualToString:@"com.apple.Preview"]) {
    return [parser keycode:@"ApplicationType::PREVIEW"];
  }

  if ([name isEqualToString:@"com.oracle.SQLDeveloper"]) {
    return [parser keycode:@"ApplicationType::ORACLE_SQL_DEVELOPER"];
  }

  if ([name isEqualToString:@"com.apple.systempreferences"]) {
    return [parser keycode:@"ApplicationType::SYSTEM_PREFERENCES"];
  }

  if ([name isEqualToString:@"com.citrix.ICAClient"]) {
    return [parser keycode:@"ApplicationType::CITRIX_ONLINE_PLUG_IN"];
  }

  if ([name isEqualToString:@"net.mame.mameosx"]) {
    return [parser keycode:@"ApplicationType::MAME"];
  }

  if ([name isEqualToString:@"org.blenderfoundation.blender"]) {
    return [parser keycode:@"ApplicationType::BLENDER"];
  }

  if ([name isEqualToString:@"com.acrylic.Pulp"]) {
    return [parser keycode:@"ApplicationType::PULP"];
  }

  if ([name isEqualToString:@"com.rsa.securid.softwaretoken"]) {
    return [parser keycode:@"ApplicationType::RSA_SECURID_SOFTWARE_TOKEN"];
  }

  if ([name isEqualToString:@"com.alfredapp.Alfred"]) {
    return [parser keycode:@"ApplicationType::ALFRED"];
  }

  if ([name isEqualToString:@"TweetDeckFast.FFF259DC0CE2657847BBB4AFF0E62062EFC56543.1"]) {
    return [parser keycode:@"ApplicationType::TWEETDECK"];
  }
  
  if ([name isEqualToString:@"com.secretgeometry.Cathode"]) {
    return [parser keycode:@"ApplicationType::CATHODE"];
  }
  
  if ([name isEqualToString:@"org.kronenberg.WineBottler_PSPad"]) {
    return [parser keycode:@"ApplicationType::PSPAD"];
  }

  return [parser keycode:@"ApplicationType::UNKNOWN"];
}

@end
