#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <Carbon/Carbon.h>

#include "server.hpp"
#include "server_objc_part.h"
#include "Mutex.hpp"

namespace {
  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::ApplicationType currentApplicationType = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::UNKNOWN;
  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::InputMode currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_NONE;
  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::InputModeDetail currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_NONE;

  Mutex mutex_currentApplicationType;
  Mutex mutex_currentInputMode;
}

bool
KeyRemap4MacBook_server::Server::initialize(const char* basedirectory)
{
  if (! basedirectory) return false;
  if (*basedirectory == '\0') return false;

  socketpath_ = std::string(basedirectory) + "/KeyRemap4MacBook_server";
  if (! makeSocket()) return false;
  if (listenSocket_ == -1) return false;

  int error = listen(listenSocket_, 128);
  if (error) return false;

  return true;
}

void
KeyRemap4MacBook_server::Server::sendReply(int sock, void* data, size_t size, uint32_t error)
{
  struct iovec iov[2];
  iov[0].iov_base = reinterpret_cast<caddr_t>(&error);
  iov[0].iov_len = sizeof(error);
  if (size > 0) {
    iov[1].iov_base = reinterpret_cast<caddr_t>(data);
    iov[1].iov_len = size;
  }

  struct msghdr msg;
  memset(&msg, 0, sizeof(msg));
  msg.msg_iov = iov;
  if (size > 0) {
    msg.msg_iovlen = 2;
  } else {
    msg.msg_iovlen = 1;
  }

  sendmsg(sock, &msg, 0);
}

void
KeyRemap4MacBook_server::Server::dispatchOperator(int sock)
{
  org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::RequestType operation;
  if (read(sock, &operation, sizeof(operation)) < 0) goto error;

  switch (operation) {
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_GET_WORKSPACE_DATA:
    {
      org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::Reply reply;
      org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error error = do_GetWorkspaceData(reply);
      sendReply(sock, &reply, sizeof(reply), error);
      break;
    }
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_CHANGE_INPUTMODE:
    {
      uint32_t size;
      if (read(sock, &size, sizeof(size)) < 0) goto error;

      org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::Request request;
      if (size != sizeof(request)) goto error;
      if (read(sock, &request, sizeof(request)) < 0) goto error;

      do_ChangeInputMode(request);

      break;
    }
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE:
    {
      uint32_t size;
      if (read(sock, &size, sizeof(size)) < 0) goto error;

      org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessage::Request request;
      if (size != sizeof(request)) goto error;
      if (read(sock, &request, sizeof(request)) < 0) goto error;

      do_StatusMessage(request);

      break;
    }
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE_WINDOW_PARAMETER:
    {
      uint32_t size;
      if (read(sock, &size, sizeof(size)) < 0) goto error;

      org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessageWindowParameter::Request request;
      if (size != sizeof(request)) goto error;
      if (read(sock, &request, sizeof(request)) < 0) goto error;

      do_StatusMessageWindowParameter(request);

      break;
    }
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_NONE:
      goto error;
  }

  return;

error:
  sendReply(sock, NULL, 0, org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ERROR);
}

void
KeyRemap4MacBook_server::Server::doLoop(void)
{
  if (listenSocket_ == -1) return;

  for (;;) {
    int s = accept(listenSocket_, NULL, NULL);
    if (s < 0) {
      return;
    }
    dispatchOperator(s);
    close(s);
  }
}

// --------------------------------------------------
bool
KeyRemap4MacBook_server::Server::makeSocket(void)
{
  listenSocket_ = socket(PF_LOCAL, SOCK_STREAM, 0);
  if (listenSocket_ < 0) return false;

  struct sockaddr_un listenSocketAddr;
  memset(&listenSocketAddr, 0, sizeof(listenSocketAddr));
  listenSocketAddr.sun_len = sizeof(listenSocketAddr);
  listenSocketAddr.sun_family = AF_UNIX;
  snprintf(listenSocketAddr.sun_path, sizeof(listenSocketAddr.sun_path), "%s", socketpath_.c_str());

  if (bind(listenSocket_, reinterpret_cast<struct sockaddr*>(&listenSocketAddr), sizeof(listenSocketAddr)) < 0) return false;

  return true;
}

// --------------------------------------------------
org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error
KeyRemap4MacBook_server::Server::do_GetWorkspaceData(org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& reply)
{
  {
    Mutex::ScopedLock lk(mutex_currentApplicationType);
    reply.type = currentApplicationType;
  }
  {
    Mutex::ScopedLock lk(mutex_currentInputMode);
    reply.inputmode = currentInputMode;
    reply.inputmodedetail = currentInputModeDetail;
  }

  // ----------------------------------------------------------------------
  return org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS;
}

org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error
KeyRemap4MacBook_server::Server::do_ChangeInputMode(const org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::Request& request)
{
  switch (request.inputmode) {
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_ENGLISH:
      selectInputSource_english();
      break;
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_FRENCH:
      selectInputSource_french();
      break;
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_GERMAN:
      selectInputSource_german();
      break;
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_JAPANESE:
      selectInputSource_japanese();
      break;
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_SWEDISH:
      selectInputSource_swedish();
      break;
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_CANADIAN:
      selectInputSource_canadian();
      break;
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::INPUTMODE_RUSSIAN:
      selectInputSource_russian();
      break;
  }
  return org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS;
}

org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error
KeyRemap4MacBook_server::Server::do_StatusMessage(const org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessage::Request& request)
{
  StatusMessageType type = STATUSMESSAGETYPE_NONE;

  switch (request.type) {
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_MODIFIER_LOCK:
      type = STATUSMESSAGETYPE_LOCK;
      break;
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_EXTRA:
      type = STATUSMESSAGETYPE_EXTRA;
      break;
    default:
      break;
  }
  set_statusmessage(type, request.message);

  return org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS;
}

org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error
KeyRemap4MacBook_server::Server::do_StatusMessageWindowParameter(const org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessageWindowParameter::Request& request)
{
  set_statusmessageWindowParam(request.alpha_font, request.alpha_background, request.posx_adjustment, request.posy_adjustment);
  return org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS;
}

// --------------------------------------------------
void
setCurrentApplicationType(const char* applicationName)
{
  Mutex::ScopedLock lk(mutex_currentApplicationType);

  // we ignore the program for our investigation.
  if (strcmp(applicationName, "org.pqrs.KeyRemap4MacBook.KeyDump") == 0) {
    // use previous value. (not set "currentApplicationType")
    return;
  }

#define SET_CURRENT_APPLICATION_TYPE(type) {                                                             \
    currentApplicationType = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::type; \
    return;                                                                                              \
}

  if (strcmp(applicationName, "org.gnu.Emacs") == 0 ||
      strcmp(applicationName, "org.gnu.AquamacsEmacs") == 0) {
    SET_CURRENT_APPLICATION_TYPE(EMACS);
  }

  const char* org_vim = "org.vim.";
  if (strncmp(applicationName, org_vim, strlen(org_vim)) == 0) {
    SET_CURRENT_APPLICATION_TYPE(VI);
  }

  if (strcmp(applicationName, "com.apple.Terminal") == 0 ||
      strcmp(applicationName, "iTerm") == 0 ||
      strcmp(applicationName, "net.sourceforge.iTerm") == 0) {
    SET_CURRENT_APPLICATION_TYPE(TERMINAL);
  }

  const char* vmware_unity = "com.vmware.proxyApp.";
  const char* parallels_coherence = "com.parallels.winapp.";
  if (strcmp(applicationName, "com.vmware.fusion") == 0 ||
      strncmp(applicationName, vmware_unity, strlen(vmware_unity)) == 0 ||
      strcmp(applicationName, "com.parallels.desktop") == 0 ||
      strcmp(applicationName, "com.parallels.vm") == 0 ||
      strcmp(applicationName, "com.parallels.desktop.console") == 0 ||
      strncmp(applicationName, parallels_coherence, strlen(parallels_coherence)) == 0 ||
      strcmp(applicationName, "org.virtualbox.app.VirtualBoxVM") == 0) {
    SET_CURRENT_APPLICATION_TYPE(VIRTUALMACHINE);
  }

  if (strcmp(applicationName, "com.microsoft.rdc") == 0 ||
      strcmp(applicationName, "net.sf.cord") == 0) {
    SET_CURRENT_APPLICATION_TYPE(REMOTEDESKTOPCONNECTION);
  }

  if (strcmp(applicationName, "org.x.X11") == 0 ||
      strcmp(applicationName, "com.apple.x11") == 0 ||
      strcmp(applicationName, "org.macosforge.xquartz.X11") == 0 ||
      strcmp(applicationName, "org.macports.X11") == 0) {
    SET_CURRENT_APPLICATION_TYPE(X11);
  }

  if (strcmp(applicationName, "com.apple.finder") == 0) {
    SET_CURRENT_APPLICATION_TYPE(FINDER);
  }

  if (strcmp(applicationName, "com.apple.Safari") == 0) {
    SET_CURRENT_APPLICATION_TYPE(SAFARI);
  }

  if (strcmp(applicationName, "org.mozilla.firefox") == 0) {
    SET_CURRENT_APPLICATION_TYPE(FIREFOX);
  }

  if (strcmp(applicationName, "org.mozilla.thunderbird") == 0) {
    SET_CURRENT_APPLICATION_TYPE(THUNDERBIRD);
  }

  if (strcmp(applicationName, "com.apple.iChat") == 0) {
    SET_CURRENT_APPLICATION_TYPE(ICHAT);
  }

  if (strcmp(applicationName, "com.adiumX.adiumX") == 0) {
    SET_CURRENT_APPLICATION_TYPE(ADIUMX);
  }

  if (strcmp(applicationName, "com.skype.skype") == 0) {
    SET_CURRENT_APPLICATION_TYPE(SKYPE);
  }

  if (strcmp(applicationName, "com.apple.mail") == 0) {
    SET_CURRENT_APPLICATION_TYPE(MAIL);
  }

  if (strcmp(applicationName, "com.apple.TextEdit") == 0) {
    SET_CURRENT_APPLICATION_TYPE(EDITOR);
  }

  if (strcmp(applicationName, "com.adobe.Photoshop") == 0 ||
      strcmp(applicationName, "com.adobe.Photoshop.Elements") == 0) {
    SET_CURRENT_APPLICATION_TYPE(PHOTOSHOP);
  }

  if (strcmp(applicationName, "com.adobe.illustrator") == 0) {
    SET_CURRENT_APPLICATION_TYPE(ILLUSTRATOR);
  }

  const char* com_adobe = "com.adobe.";
  if (strncmp(applicationName, com_adobe, strlen(com_adobe)) == 0) {
    SET_CURRENT_APPLICATION_TYPE(ADOBE);
  }

  if (strcmp(applicationName, "com.microsoft.Excel") == 0) {
    SET_CURRENT_APPLICATION_TYPE(EXCEL);
  }

  if (strcmp(applicationName, "com.microsoft.Entourage") == 0) {
    SET_CURRENT_APPLICATION_TYPE(ENTOURAGE);
  }

  if (strcmp(applicationName, "org.eclipse.eclipse") == 0) {
    SET_CURRENT_APPLICATION_TYPE(ECLIPSE);
  }

  if (strcmp(applicationName, "org.pqrs.unknownapp.MacSOUP") == 0) {
    SET_CURRENT_APPLICATION_TYPE(MACSOUP);
  }

  if (strcmp(applicationName, "uk.co.opencommunity.vienna2") == 0) {
    SET_CURRENT_APPLICATION_TYPE(VIENNA);
  }

  if (strcmp(applicationName, "com.apple.Xcode") == 0) {
    SET_CURRENT_APPLICATION_TYPE(XCODE);
  }

  if (strcmp(applicationName, "com.operasoftware.Opera") == 0) {
    SET_CURRENT_APPLICATION_TYPE(OPERA);
  }

  if (strcmp(applicationName, "com.google.Chrome") == 0) {
    SET_CURRENT_APPLICATION_TYPE(GOOGLE_CHROME);
  }

  if (strcmp(applicationName, "com.TeamViewer.TeamViewer") == 0) {
    SET_CURRENT_APPLICATION_TYPE(TEAMVIEWER);
  }

  if (strcmp(applicationName, "de.wengenmayer.Cheetah3D") == 0) {
    SET_CURRENT_APPLICATION_TYPE(CHEETAH_3D);
  }

  if (strcmp(applicationName, "com.apple.securityagent") == 0) {
    SET_CURRENT_APPLICATION_TYPE(PASSWORD);
  }

  if (strcmp(applicationName, "com.sakya.Gruml") == 0) {
    SET_CURRENT_APPLICATION_TYPE(GRUML);
  }

  if (strcmp(applicationName, "com.apple.iPhoto") == 0) {
    SET_CURRENT_APPLICATION_TYPE(IPHOTO);
  }

  if (strcmp(applicationName, "com.apple.Stickies") == 0) {
    SET_CURRENT_APPLICATION_TYPE(STICKIES);
  }

  SET_CURRENT_APPLICATION_TYPE(UNKNOWN);
}

void
setCurrentInputMode(const char* inputmodeName)
{
  Mutex::ScopedLock lk(mutex_currentInputMode);

  // ----------------------------------------------------------------------
  // inputmode
  // get data from util/DumpInputModeToConsole/dump-from-plist.sh
  const char* tis_japanese_hiragana = "com.apple.inputmethod.Japanese.Hiragana";
  const char* tis_japanese_katakana = "com.apple.inputmethod.Japanese.Katakana";
  const char* tis_japanese_fullwidth_roman = "com.apple.inputmethod.Japanese.FullWidthRoman";
  const char* tis_japanese_halfwidth_kana = "com.apple.inputmethod.Japanese.HalfWidthKana";
  const char* tis_japanese = "com.apple.inputmethod.Japanese";
  const char* tis_tradchinese = "com.apple.inputmethod.TCIM"; // TradChinese
  const char* tis_simpchinese = "com.apple.inputmethod.SCIM"; // SimpChinese
  const char* tis_korean = "com.apple.inputmethod.Korean";
  const char* language_swedish = "org.pqrs.inputmode.sv.";
  const char* language_canadian = "org.pqrs.inputmode.ca.";
  const char* language_ainu = "com.apple.kotoeri.Ainu";
  const char* language_russian = "org.pqrs.inputmode.ru.";
  const char* language_french = "org.pqrs.inputmode.fr.";
  const char* language_unknown = "org.pqrs.inputmode.unknown.";

  if (strcmp(inputmodeName, tis_japanese_hiragana) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA;

  } else if (strcmp(inputmodeName, tis_japanese_katakana) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_KATAKANA;

  } else if (strcmp(inputmodeName, tis_japanese_fullwidth_roman) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_FULLWIDTH_ROMAN;

  } else if (strcmp(inputmodeName, tis_japanese_halfwidth_kana) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HALFWIDTH_KANA;

  } else if (strncmp(inputmodeName, tis_japanese, strlen(tis_japanese)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE;

  } else if (strncmp(inputmodeName, tis_tradchinese, strlen(tis_tradchinese)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_CHINESE_TRADITIONAL;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_CHINESE_TRADITIONAL;

  } else if (strncmp(inputmodeName, tis_simpchinese, strlen(tis_simpchinese)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_CHINESE_SIMPLIFIED;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_CHINESE_SIMPLIFIED;

  } else if (strncmp(inputmodeName, tis_korean, strlen(tis_korean)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_KOREAN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_KOREAN;

  } else if (strncmp(inputmodeName, language_swedish, strlen(language_swedish)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_SWEDISH;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_SWEDISH;

  } else if (strncmp(inputmodeName, language_canadian, strlen(language_canadian)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_CANADIAN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_CANADIAN;

  } else if (strcmp(inputmodeName, language_ainu) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_AINU;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_AINU;

  } else if (strncmp(inputmodeName, language_russian, strlen(language_russian)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_RUSSIAN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_RUSSIAN;

  } else if (strncmp(inputmodeName, language_french, strlen(language_french)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_FRENCH;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_FRENCH;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorak") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmd") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO_AZERTYCMD;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorak-AzertyCmdRoman") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO_AZERTYCMDROMAN;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmd") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO_QWERTYCMD;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorak-QwertyCmdRoman") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO_QWERTYCMDROMAN;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmd") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO_QWERTZCMD;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorak-QwertzCmdRoman") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO_QWERTZCMDROMAN;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.FrenchDvorakRoman") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_BEPO;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_BEPO_ROMAN;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.en.Dvorak") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DVORAK;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_DVORAK;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.en.Dvorak-Left") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DVORAK;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_DVORAK_LEFT;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.en.DVORAK-QWERTYCMD") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DVORAK;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_DVORAK_QWERTYCMD;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.en.Dvorak-Right") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DVORAK;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_DVORAK_RIGHT;

  } else if (strcmp(inputmodeName, "org.pqrs.inputmode.unknown.JANSI") == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_ROMAN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN_JANSI;

  } else if (strncmp(inputmodeName, language_unknown, strlen(language_unknown)) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_UNKNOWN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_UNKNOWN;

  } else {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_ROMAN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN;
  }
}
