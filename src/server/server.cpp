#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <stdio.h>

#include "getActiveApplicationName.h"
#include "server.hpp"

void
KeyRemap4MacBook_server::Server::initialize(void)
{
  // --------------------
  // setup thread pool
  pthread_attr_t attr;
  pthread_t thread;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  for (int i = 0; i < NUM_THREAD; ++i) {
    pthread_create(&thread, &attr, handleRequestThread, this);
  }

  // --------------------
  makeSocket();
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
    case KeyRemap4MacBook_server::Bridge::REQUEST_TYPE_ACTIVE_APPLICATION_INFO:
    {
      KeyRemap4MacBook_server::Bridge::ActiveApplicationInfo::Reply reply;
      KeyRemap4MacBook_server::Bridge::ReplyType error = do_ActiveApplicationInfo(&reply);
      sendReply(sock, &reply, sizeof(reply), error);
      break;
    }
    default:
      goto error;
  }

  return;

error:
  sendReply(sock, NULL, 0, KeyRemap4MacBook_server::Bridge::REPLYTYPE_ERROR);
}

void *
KeyRemap4MacBook_server::Server::handleRequestThread(void *arg)
{
  KeyRemap4MacBook_server::Server *serv = reinterpret_cast<KeyRemap4MacBook_server::Server *>(arg);

  KeyRemap4MacBook_server::Thread::Mutex &mutex = (serv->requestsMutex);
  KeyRemap4MacBook_server::Thread::Cond &cond = (serv->requestCond);
  std::queue<int> &queue = (serv->queueRequestSocket);

  for (;;) {
    mutex.lock();
    if (queue.empty()) {
      cond.wait(&mutex);
      mutex.unlock();
      continue;

    } else {
      int sock = queue.front();
      queue.pop();

      mutex.unlock();
      (serv->dispatchOperator)(sock);
      close(sock);
    }
  }

  return NULL;
}

bool
KeyRemap4MacBook_server::Server::enqueueRequest(int sock)
{
  requestsMutex.lock();

  queueRequestSocket.push(sock);
  requestCond.signal();

  requestsMutex.unlock();
  return true;
}

void
KeyRemap4MacBook_server::Server::doLoop(void)
{
  initialize();

  listen(listenSocket, 128);

  exitLoopFlag = false;
  while (! exitLoopFlag) {
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
      enqueueRequest(s);
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
  listenSocketAddr.sun_family = AF_LOCAL;
  snprintf(listenSocketAddr.sun_path, sizeof(listenSocketAddr.sun_path), KeyRemap4MacBook_server::Bridge::socketPath);

  if (bind(listenSocket, reinterpret_cast<struct sockaddr *>(&listenSocketAddr), sizeof(listenSocketAddr)) < 0) return false;

  return true;
}

void
KeyRemap4MacBook_server::Server::deleteSocket(void)
{
  unlink(KeyRemap4MacBook_server::Bridge::socketPath);
}


// --------------------------------------------------
KeyRemap4MacBook_server::Bridge::ReplyType
KeyRemap4MacBook_server::Server::do_ActiveApplicationInfo(KeyRemap4MacBook_server::Bridge::ActiveApplicationInfo::Reply *reply)
{
  if (! reply) return KeyRemap4MacBook_server::Bridge::REPLYTYPE_ERROR;

  char applicationName[128];
  getActiveApplicationName(applicationName, sizeof(applicationName));

  reply->is_emacs = false;
  reply->is_terminal = false;
  reply->is_virtualmachine = false;

  printf("%s\n", applicationName);

  if (strcmp(applicationName, "org.gnu.Emacs") == 0) {
    reply->is_emacs = true;
  }
  if (strcmp(applicationName, "com.apple.Terminal") == 0) {
    reply->is_terminal = true;
  }
  if (strcmp(applicationName, "com.vmware.fusion") == 0) {
    reply->is_virtualmachine = true;
  }

  return KeyRemap4MacBook_server::Bridge::REPLYTYPE_SUCCESS;
}
