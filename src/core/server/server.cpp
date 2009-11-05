#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>
#include <Carbon/Carbon.h>

#include "server.hpp"
#include "server_objc_part.h"

bool
KeyRemap4MacBook_server::Server::initialize(const char* basedirectory)
{
  if (! basedirectory) return false;
  if (*basedirectory == '\0') return false;

  socketpath_ = std::string(basedirectory) + "/KeyRemap4MacBook_server";
  return makeSocket();
}

void
KeyRemap4MacBook_server::Server::sendReply(int sock, void *data, size_t size, int error)
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
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(listenSocket_, &readfds);

    int nfds = listenSocket_ + 1;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (select(nfds, &readfds, NULL, NULL, &timeout) == -1) {
      return;
    }

    if (FD_ISSET(listenSocket_, &readfds)) {
      struct sockaddr_un addr;
      socklen_t addrlen;

      int s = accept(listenSocket_, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);
      if (s < 0) {
        return;
      }
      dispatchOperator(s);
      close(s);
    }
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

  if (bind(listenSocket_, reinterpret_cast<struct sockaddr *>(&listenSocketAddr), sizeof(listenSocketAddr)) < 0) return false;

  return true;
}

// --------------------------------------------------
org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error
KeyRemap4MacBook_server::Server::do_GetWorkspaceData(org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& reply)
{
  char applicationName[128];
  char inputmodeName[128];
  autoreleasepool_begin();
  getActiveApplicationName(applicationName, sizeof(applicationName));
  getTISPropertyInputModeID(inputmodeName, sizeof(inputmodeName));
  autoreleasepool_end();

  reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::UNKNOWN;
  reply.inputmode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_ROMAN;
  reply.inputmodedetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN;

  // ----------------------------------------------------------------------
  // type
  const char *org_vim = "org.vim.";
  const char *com_adobe = "com.adobe.";

  if (strcmp(applicationName, "org.gnu.Emacs") == 0 ||
      strcmp(applicationName, "org.gnu.AquamacsEmacs") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::EMACS;

  } else if (strncmp(applicationName, org_vim, strlen(org_vim)) == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::VI;

  } else if (strcmp(applicationName, "com.apple.Terminal") == 0 ||
             strcmp(applicationName, "iTerm") == 0 ||
             strcmp(applicationName, "net.sourceforge.iTerm") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::TERMINAL;

  } else if (strcmp(applicationName, "com.vmware.fusion") == 0 ||
             strcmp(applicationName, "com.parallels.desktop") == 0 ||
             strcmp(applicationName, "org.virtualbox.app.VirtualBoxVM") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::VIRTUALMACHINE;

  } else if (strcmp(applicationName, "com.microsoft.rdc") == 0 ||
             strcmp(applicationName, "net.sf.cord") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::REMOTEDESKTOPCONNECTION;

  } else if (strcmp(applicationName, "org.x.X11") == 0 ||
             strcmp(applicationName, "com.apple.x11") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::X11;

  } else if (strcmp(applicationName, "com.apple.finder") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::FINDER;

  } else if (strcmp(applicationName, "com.apple.Safari") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::SAFARI;

  } else if (strcmp(applicationName, "org.mozilla.firefox") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::FIREFOX;

  } else if (strcmp(applicationName, "org.mozilla.thunderbird") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::THUNDERBIRD;

  } else if (strcmp(applicationName, "com.apple.iChat") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::ICHAT;

  } else if (strcmp(applicationName, "com.adiumX.adiumX") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::ADIUMX;

  } else if (strcmp(applicationName, "com.skype.skype") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::SKYPE;

  } else if (strcmp(applicationName, "com.apple.mail") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::MAIL;

  } else if (strcmp(applicationName, "com.apple.TextEdit") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::EDITOR;

  } else if (strncmp(applicationName, com_adobe, strlen(com_adobe)) == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::ADOBE;

  } else if (strcmp(applicationName, "com.microsoft.Excel") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::EXCEL;

  } else if (strcmp(applicationName, "com.microsoft.Entourage") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::ENTOURAGE;

  } else if (strcmp(applicationName, "org.eclipse.eclipse") == 0) {
    reply.type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::ECLIPSE;
  }

  // ----------------------------------------------------------------------
  // inputmode
  const char* tis_japanese_hiragana = CFStringGetCStringPtr(kTextServiceInputModeJapaneseHiragana, kCFStringEncodingUTF8);
  const char* tis_japanese_katakana = CFStringGetCStringPtr(kTextServiceInputModeJapaneseKatakana, kCFStringEncodingUTF8);
  const char* tis_japanese = CFStringGetCStringPtr(kTextServiceInputModeJapanese, kCFStringEncodingUTF8);

  if (strcmp(inputmodeName, tis_japanese_hiragana) == 0) {
    reply.inputmode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    reply.inputmodedetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_HIRAGANA;

  } else if (strcmp(inputmodeName, tis_japanese_katakana) == 0) {
    reply.inputmode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    reply.inputmodedetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE_KATAKANA;

  } else if (strncmp(inputmodeName, tis_japanese, strlen(tis_japanese)) == 0) {
    reply.inputmode = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_JAPANESE;
    reply.inputmodedetail = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_JAPANESE;
  }

  // ----------------------------------------------------------------------
  return org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS;
}
