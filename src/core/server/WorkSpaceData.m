// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "WorkSpaceData.h"

@implementation WorkSpaceData

+ (unsigned int) getApplicationType:(NSDictionary*)dict name:(NSString*)name
{
  if ([name isEqualToString:@"org.gnu.Emacs"] ||
      [name isEqualToString:@"org.gnu.AquamacsEmacs"]) {
    return [[dict objectForKey:@"ApplicationType::EMACS"] unsignedIntValue];
  }

  if ([name hasPrefix:@"org.vim."]) {
    return [[dict objectForKey:@"ApplicationType::VI"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.Terminal"]    ||
      [name isEqualToString:@"iTerm"]                 ||
      [name isEqualToString:@"net.sourceforge.iTerm"] ||
      [name isEqualToString:@"com.googlecode.iterm2"]) {
    return [[dict objectForKey:@"ApplicationType::TERMINAL"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.vmware.fusion"]             ||
      [name hasPrefix:@"com.vmware.proxyApp."]                || // unity
      [name isEqualToString:@"com.parallels.desktop"]         ||
      [name isEqualToString:@"com.parallels.vm"]              ||
      [name isEqualToString:@"com.parallels.desktop.console"] ||
      [name hasPrefix:@"com.parallels.winapp."]               || // coherence
      [name isEqualToString:@"org.virtualbox.app.VirtualBoxVM"]) {
    return [[dict objectForKey:@"ApplicationType::VIRTUALMACHINE"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.microsoft.rdc"] ||
      [name isEqualToString:@"net.sf.cord"]) {
    return [[dict objectForKey:@"ApplicationType::REMOTEDESKTOPCONNECTION"] unsignedIntValue];
  }

  if ([name isEqualToString:@"org.x.X11"] ||
      [name isEqualToString:@"com.apple.x11"] ||
      [name isEqualToString:@"org.macosforge.xquartz.X11"] ||
      [name isEqualToString:@"org.macports.X11"]) {
    return [[dict objectForKey:@"ApplicationType::X11"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.finder"]) {
    return [[dict objectForKey:@"ApplicationType::FINDER"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.Safari"]) {
    return [[dict objectForKey:@"ApplicationType::SAFARI"] unsignedIntValue];
  }

  if ([name isEqualToString:@"org.mozilla.firefox"]) {
    return [[dict objectForKey:@"ApplicationType::FIREFOX"] unsignedIntValue];
  }

  if ([name isEqualToString:@"org.mozilla.thunderbird"]) {
    return [[dict objectForKey:@"ApplicationType::THUNDERBIRD"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.iChat"]) {
    return [[dict objectForKey:@"ApplicationType::ICHAT"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.adiumX.adiumX"]) {
    return [[dict objectForKey:@"ApplicationType::ADIUMX"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.skype.skype"]) {
    return [[dict objectForKey:@"ApplicationType::SKYPE"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.mail"]) {
    return [[dict objectForKey:@"ApplicationType::MAIL"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.TextEdit"]) {
    return [[dict objectForKey:@"ApplicationType::EDITOR"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.adobe.Photoshop"] ||
      [name isEqualToString:@"com.adobe.Photoshop.Elements"]) {
    return [[dict objectForKey:@"ApplicationType::PHOTOSHOP"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.adobe.illustrator"]) {
    return [[dict objectForKey:@"ApplicationType::ILLUSTRATOR"] unsignedIntValue];
  }

  if ([name hasPrefix:@"com.adobe."]) {
    return [[dict objectForKey:@"ApplicationType::ADOBE"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.microsoft.Excel"]) {
    return [[dict objectForKey:@"ApplicationType::EXCEL"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.microsoft.Entourage"]) {
    return [[dict objectForKey:@"ApplicationType::ENTOURAGE"] unsignedIntValue];
  }

  if ([name isEqualToString:@"org.eclipse.eclipse"]) {
    return [[dict objectForKey:@"ApplicationType::ECLIPSE"] unsignedIntValue];
  }

  if ([name isEqualToString:@"org.pqrs.unknownapp.MacSOUP"]) {
    return [[dict objectForKey:@"ApplicationType::MACSOUP"] unsignedIntValue];
  }

  if ([name isEqualToString:@"uk.co.opencommunity.vienna2"]) {
    return [[dict objectForKey:@"ApplicationType::VIENNA"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.Xcode"]) {
    return [[dict objectForKey:@"ApplicationType::XCODE"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.operasoftware.Opera"]) {
    return [[dict objectForKey:@"ApplicationType::OPERA"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.google.Chrome"]) {
    return [[dict objectForKey:@"ApplicationType::GOOGLE_CHROME"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.TeamViewer.TeamViewer"]) {
    return [[dict objectForKey:@"ApplicationType::TeamViewer"] unsignedIntValue];
  }

  if ([name isEqualToString:@"de.wengenmayer.Cheetah3D"]) {
    return [[dict objectForKey:@"ApplicationType::CHEETAH_3D"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.securityagent"]) {
    return [[dict objectForKey:@"ApplicationType::PASSWORD"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.sakya.Gruml"]) {
    return [[dict objectForKey:@"ApplicationType::GRUML"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.iPhoto"]) {
    return [[dict objectForKey:@"ApplicationType::IPHOTO"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.Stickies"]) {
    return [[dict objectForKey:@"ApplicationType::STICKIES"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.jetbrains.webide"]) {
    return [[dict objectForKey:@"ApplicationType::WEBIDE"] unsignedIntValue];
  }

  if ([name isEqualToString:@"at.obdev.LaunchBar"]) {
    return [[dict objectForKey:@"ApplicationType::LAUNCHBAR"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.apple.ScreenSharing"]) {
    return [[dict objectForKey:@"ApplicationType::SCREEN_SHARING"] unsignedIntValue];
  }

  if ([name isEqualToString:@"com.geekspiff.chickenofthevnc"]) {
    return [[dict objectForKey:@"ApplicationType::VNC"] unsignedIntValue];
  }

  return [[dict objectForKey:@"ApplicationType::UNKNOWN"] unsignedIntValue];
}

@end
