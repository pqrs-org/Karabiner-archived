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
  return makeSocket();
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
  int operation;
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
    default:
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

  int error = listen(listenSocket_, 128);
  if (error) return;

  for (;;) {
    struct sockaddr_un addr;
    socklen_t addrlen = sizeof(addr);

    int s = accept(listenSocket_, reinterpret_cast<struct sockaddr*>(&addr), &addrlen);
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
      strcmp(applicationName, "com.apple.x11") == 0) {
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
  const char* tis_japanese = "com.apple.inputmethod.Japanese";
  const char* tis_tradchinese = "com.apple.inputmethod.TCIM"; // TradChinese
  const char* tis_simpchinese = "com.apple.inputmethod.SCIM"; // SimpChinese
  const char* tis_korean = "com.apple.inputmethod.Korean";
  const char* language_swedish = "org.pqrs.inputmode.sv";
  const char* language_canadian = "org.pqrs.inputmode.ca";

  if (strcmp(inputmodeName, tis_japanese_hiragana) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA;

  } else if (strcmp(inputmodeName, tis_japanese_katakana) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_KATAKANA;

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

  } else if (strcmp(inputmodeName, language_swedish) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_SWEDISH;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_SWEDISH;

  } else if (strcmp(inputmodeName, language_canadian) == 0) {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_CANADIAN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_CANADIAN;

  } else {
    currentInputMode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_ROMAN;
    currentInputModeDetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN;
  }
}
