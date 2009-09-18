#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

#include "getActiveApplicationName.h"
#include "server.hpp"

bool
KeyRemap4MacBook_server::Server::initialize(void)
{
  return makeSocket();
}

void
KeyRemap4MacBook_server::Server::terminate(void)
{
  deleteSocket();
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
    case org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::REQUEST_ACTIVE_APPLICATION_INFO:
    {
      org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::Reply reply;
      org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error error = do_ActiveApplicationInfo(&reply);
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
  if (! initialize()) return;

  // no more root privilege
  seteuid(getuid());

  listen(listenSocket, 128);

  for (;;) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(listenSocket, &readfds);

    int nfds = listenSocket + 1;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (select(nfds, &readfds, NULL, NULL, &timeout) == -1) {
      return;
    }

    if (FD_ISSET(listenSocket, &readfds)) {
      struct sockaddr_un addr;
      socklen_t addrlen;

      int s = accept(listenSocket, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);
      if (s < 0) {
        return;
      }
      dispatchOperator(s);
      close(s);
    }
  }

  terminate();
}

// --------------------------------------------------
bool
KeyRemap4MacBook_server::Server::makeSocket(void)
{
  deleteSocket();

  listenSocket = socket(PF_LOCAL, SOCK_STREAM, 0);
  if (listenSocket < 0) return false;

  struct sockaddr_un listenSocketAddr;
  memset(&listenSocketAddr, 0, sizeof(listenSocketAddr));
  listenSocketAddr.sun_len = sizeof(listenSocketAddr);
  listenSocketAddr.sun_family = AF_UNIX;
  snprintf(listenSocketAddr.sun_path, sizeof(listenSocketAddr.sun_path), org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::socketPath);

  if (bind(listenSocket, reinterpret_cast<struct sockaddr *>(&listenSocketAddr), sizeof(listenSocketAddr)) < 0) return false;

  return true;
}

void
KeyRemap4MacBook_server::Server::deleteSocket(void)
{
  unlink(org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::socketPath);
}


// --------------------------------------------------
org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error
KeyRemap4MacBook_server::Server::do_ActiveApplicationInfo(org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::Reply *reply)
{
  if (! reply) return org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ERROR;

  char applicationName[128];
  getActiveApplicationName(applicationName, sizeof(applicationName));

  reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::UNKNOWN;

  const char *org_vim = "org.vim.";
  const char *com_adobe = "com.adobe.";

  if (strcmp(applicationName, "org.gnu.Emacs") == 0 ||
      strcmp(applicationName, "org.gnu.AquamacsEmacs") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::EMACS;

  } else if (strncmp(applicationName, org_vim, strlen(org_vim)) == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::VI;

  } else if (strcmp(applicationName, "com.apple.Terminal") == 0 ||
             strcmp(applicationName, "iTerm") == 0 ||
             strcmp(applicationName, "net.sourceforge.iTerm") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::TERMINAL;

  } else if (strcmp(applicationName, "com.vmware.fusion") == 0 ||
             strcmp(applicationName, "com.parallels.desktop") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE;

  } else if (strcmp(applicationName, "com.microsoft.rdc") == 0 ||
             strcmp(applicationName, "net.sf.cord") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::REMOTEDESKTOPCONNECTION;

  } else if (strcmp(applicationName, "org.x.X11") == 0 ||
             strcmp(applicationName, "com.apple.x11") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::X11;

  } else if (strcmp(applicationName, "com.apple.finder") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::FINDER;

  } else if (strcmp(applicationName, "com.apple.Safari") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::SAFARI;

  } else if (strcmp(applicationName, "org.mozilla.firefox") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::FIREFOX;

  } else if (strcmp(applicationName, "org.mozilla.thunderbird") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::THUNDERBIRD;

  } else if (strcmp(applicationName, "com.apple.iChat") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::ICHAT;

  } else if (strcmp(applicationName, "com.adiumX.adiumX") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::ADIUMX;

  } else if (strcmp(applicationName, "com.skype.skype") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::SKYPE;

  } else if (strcmp(applicationName, "com.apple.mail") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::MAIL;

  } else if (strcmp(applicationName, "com.apple.TextEdit") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::EDITOR;

  } else if (strncmp(applicationName, com_adobe, strlen(com_adobe)) == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::ADOBE;

  } else if (strcmp(applicationName, "com.microsoft.Excel") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::EXCEL;

  } else if (strcmp(applicationName, "com.microsoft.Entourage") == 0) {
    reply->type = org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::ENTOURAGE;
  }

  return org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::SUCCESS;
}
