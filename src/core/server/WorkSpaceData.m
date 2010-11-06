// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "WorkSpaceData.h"

@implementation WorkSpaceData

- (unsigned int) getApplicationType:(NSString*)name
{
  if (! name) return 0;

  if ([name isEqualToString:@"org.gnu.Emacs"] ||
      [name isEqualToString:@"org.gnu.AquamacsEmacs"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::EMACS"];
  }

  if ([name hasPrefix:@"org.vim."]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::VI"];
  }

  if ([name isEqualToString:@"com.apple.Terminal"]    ||
      [name isEqualToString:@"iTerm"]                 ||
      [name isEqualToString:@"net.sourceforge.iTerm"] ||
      [name isEqualToString:@"com.googlecode.iterm2"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::TERMINAL"];
  }

  if ([name isEqualToString:@"com.vmware.fusion"]             ||
      [name hasPrefix:@"com.vmware.proxyApp."]                || // unity
      [name isEqualToString:@"com.parallels.desktop"]         ||
      [name isEqualToString:@"com.parallels.vm"]              ||
      [name isEqualToString:@"com.parallels.desktop.console"] ||
      [name hasPrefix:@"com.parallels.winapp."]               || // coherence
      [name isEqualToString:@"org.virtualbox.app.VirtualBoxVM"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::VIRTUALMACHINE"];
  }

  if ([name isEqualToString:@"com.microsoft.rdc"] ||
      [name isEqualToString:@"net.sf.cord"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::REMOTEDESKTOPCONNECTION"];
  }

  if ([name isEqualToString:@"org.x.X11"] ||
      [name isEqualToString:@"com.apple.x11"] ||
      [name isEqualToString:@"org.macosforge.xquartz.X11"] ||
      [name isEqualToString:@"org.macports.X11"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::X11"];
  }

  if ([name isEqualToString:@"com.apple.finder"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::FINDER"];
  }

  if ([name isEqualToString:@"com.apple.Safari"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::SAFARI"];
  }

  if ([name isEqualToString:@"org.mozilla.firefox"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::FIREFOX"];
  }

  if ([name isEqualToString:@"org.mozilla.thunderbird"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::THUNDERBIRD"];
  }

  if ([name isEqualToString:@"com.apple.iChat"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::ICHAT"];
  }

  if ([name isEqualToString:@"com.adiumX.adiumX"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::ADIUMX"];
  }

  if ([name isEqualToString:@"com.skype.skype"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::SKYPE"];
  }

  if ([name isEqualToString:@"com.apple.mail"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::MAIL"];
  }

  if ([name isEqualToString:@"com.apple.TextEdit"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::EDITOR"];
  }

  if ([name isEqualToString:@"com.adobe.Photoshop"] ||
      [name isEqualToString:@"com.adobe.Photoshop.Elements"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::PHOTOSHOP"];
  }

  if ([name isEqualToString:@"com.adobe.illustrator"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::ILLUSTRATOR"];
  }

  if ([name hasPrefix:@"com.adobe."]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::ADOBE"];
  }

  if ([name isEqualToString:@"com.microsoft.Excel"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::EXCEL"];
  }

  if ([name isEqualToString:@"com.microsoft.Entourage"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::ENTOURAGE"];
  }

  if ([name isEqualToString:@"org.eclipse.eclipse"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::ECLIPSE"];
  }

  if ([name isEqualToString:@"org.pqrs.unknownapp.MacSOUP"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::MACSOUP"];
  }

  if ([name isEqualToString:@"uk.co.opencommunity.vienna2"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::VIENNA"];
  }

  if ([name isEqualToString:@"com.apple.Xcode"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::XCODE"];
  }

  if ([name isEqualToString:@"com.operasoftware.Opera"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::OPERA"];
  }

  if ([name isEqualToString:@"com.google.Chrome"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::GOOGLE_CHROME"];
  }

  if ([name isEqualToString:@"com.TeamViewer.TeamViewer"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::TEAMVIEWER"];
  }

  if ([name isEqualToString:@"de.wengenmayer.Cheetah3D"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::CHEETAH_3D"];
  }

  if ([name isEqualToString:@"com.apple.securityagent"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::PASSWORD"];
  }

  if ([name isEqualToString:@"com.sakya.Gruml"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::GRUML"];
  }

  if ([name isEqualToString:@"com.apple.iPhoto"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::IPHOTO"];
  }

  if ([name isEqualToString:@"com.apple.Stickies"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::STICKIES"];
  }

  if ([name isEqualToString:@"com.jetbrains.webide"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::WEBIDE"];
  }

  if ([name isEqualToString:@"at.obdev.LaunchBar"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::LAUNCHBAR"];
  }

  if ([name isEqualToString:@"com.apple.ScreenSharing"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::SCREEN_SHARING"];
  }

  if ([name isEqualToString:@"com.geekspiff.chickenofthevnc"]) {
    return [keycode_ unsignedIntValue:@"ApplicationType::VNC"];
  }

  return [keycode_ unsignedIntValue:@"ApplicationType::UNKNOWN"];
}

- (void) getInputMode:(NSString*)name output:(struct InputModeInfo*)output
{
  if (! name || ! output) return;

  // get data from KeyDump.app

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.Hiragana"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::JAPANESE"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::JAPANESE_HIRAGANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.Katakana"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::JAPANESE"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::JAPANESE_KATAKANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.FullWidthRoman"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::JAPANESE"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::JAPANESE_FULLWIDTH_ROMAN"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese.HalfWidthKana"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::JAPANESE"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::JAPANESE_HALFWIDTH_KANA"];
    return;
  }

  if ([name isEqualToString:@"com.apple.inputmethod.Japanese"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::JAPANESE"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::JAPANESE"];
    return;
  }

  // TradChinese
  if ([name hasPrefix:@"com.apple.inputmethod.TCIM"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::CHINESE_TRADITIONAL"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::CHINESE_TRADITIONAL"];
    return;
  }

  // SimpChinese
  if ([name hasPrefix:@"com.apple.inputmethod.SCIM"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::CHINESE_SIMPLIFIED"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::CHINESE_SIMPLIFIED"];
    return;
  }

  if ([name hasPrefix:@"com.apple.inputmethod.Korean"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::KOREAN"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::KOREAN"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.sv."]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::SWEDISH"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::SWEDISH"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.ca."]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::CANADIAN"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::CANADIAN"];
    return;
  }

  if ([name isEqualToString:@"com.apple.kotoeri.Ainu"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::AINU"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::AINU"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.ru."] ||
      [name isEqualToString:@"org.pqrs.inputmode.unknown.RussianWin"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::RUSSIAN"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::RUSSIAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.Russian-IlyaBirmanTypography"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::RUSSIAN_TYPOGRAPHIC"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::RUSSIAN_TYPOGRAPHIC"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.English-IlyaBirmanTypography"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::ENGLISH_TYPOGRAPHIC"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::ENGLISH_TYPOGRAPHIC"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.fr."]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::FRENCH"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::FRENCH"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmd"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO_AZERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmdRoman"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO_AZERTYCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmd"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO_QWERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmdRoman"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO_QWERTYCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmd"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO_QWERTZCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmdRoman"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO_QWERTZCMDROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.FrenchDvorakRoman"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::BEPO"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::BEPO_ROMAN"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::DVORAK"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::DVORAK"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak-Left"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::DVORAK"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::DVORAK_LEFT"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.DVORAK-QWERTYCMD"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::DVORAK"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::DVORAK_QWERTYCMD"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.en.Dvorak-Right"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::DVORAK"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::DVORAK_RIGHT"];
    return;
  }

  if ([name isEqualToString:@"org.pqrs.inputmode.unknown.JANSI"]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::ROMAN"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::ROMAN_JANSI"];
    return;
  }

  if ([name hasPrefix:@"org.pqrs.inputmode.unknown."]) {
    output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::UNKNOWN"];
    output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::UNKNOWN"];
    return;
  }

  output->inputmode       = [keycode_ unsignedIntValue:@"InputMode::ROMAN"];
  output->inputmodedetail = [keycode_ unsignedIntValue:@"InputModeDetail::ROMAN"];
}

@end
