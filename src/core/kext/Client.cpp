extern "C" {
#include <sys/systm.h>
#include <sys/un.h>
#include <sys/kpi_socket.h>
errno_t sock_nointerrupt(socket_t so, int on);
}
#include "base.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    struct sockaddr_un sockaddr_;
    bool sockaddr_available_ = false;
    IOLock* lock_ = NULL;

    void
    releaseSocket(socket_t& socket)
    {
      sock_shutdown(socket, SHUT_RDWR);
      sock_close(socket);
    }

    bool
    makeSocket(socket_t& socket)
    {
      int error = sock_socket(PF_LOCAL, SOCK_STREAM, 0, NULL, NULL, &socket);
      if (error) {
        IOLOG_ERROR("Client::makeSocket sock_socket failed error:%d\n", error);
        return false;
      }

      // ----------------------------------------
      struct timeval tv;
      tv.tv_sec = KeyRemap4MacBook_client::TIMEOUT_SECOND;
      tv.tv_usec = KeyRemap4MacBook_client::TIMEOUT_MICROSECOND;

      error = sock_setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
      if (error) {
        IOLOG_ERROR("Client::makeSocket sock_setsockopt failed error:%d\n", error);
        goto error;
      }

      error = sock_setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval));
      if (error) {
        IOLOG_ERROR("Client::makeSocket sock_setsockopt failed error:%d\n", error);
        goto error;
      }

      return true;

    error:
      releaseSocket(socket);
      IOLOG_ERROR("Client::makeSocket failed error:%d\n", error);

      return false;
    }

    bool
    connectSocket(socket_t& socket)
    {
      if (! sockaddr_available_) return false;

      errno_t error = sock_connect(socket, reinterpret_cast<const sockaddr*>(&sockaddr_), 0);
      if (error) {
#if 0
        // the connection failure is no problem because a server does not start at login window.
        IOLOG_ERROR("Client::connectSocket sock_connect failed error:%d\n", error);
#endif
        return false;
      }
      error = sock_nointerrupt(socket, TRUE);
      if (error) {
        IOLOG_ERROR("Client::connectSocket sock_nointerrupt failed error:%d\n", error);
        return false;
      }

      return true;
    }
  }

  void
  KeyRemap4MacBook_client::initialize(void)
  {
    lock_ = IOLockWrapper::alloc();
    refreshSockAddr();
  }

  void
  KeyRemap4MacBook_client::terminate(void)
  {
    if (lock_) {
      IOLockWrapper::free(lock_);
    }
  }

  void
  KeyRemap4MacBook_client::refreshSockAddr(void)
  {
    if (! lock_) return;
    IOLockWrapper::ScopedLock lk(lock_);

    if (Config::socket_path[0] == '\0') {
      sockaddr_available_ = false;
    } else {
      sockaddr_available_ = true;

      memset(&sockaddr_, 0, sizeof(sockaddr_));
      sockaddr_.sun_len = sizeof(sockaddr_);
      sockaddr_.sun_family = AF_UNIX;
      strlcpy(sockaddr_.sun_path, Config::socket_path, sizeof(sockaddr_.sun_path) - 8);
    }
  }

  int
  KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::RequestType type, void* request, uint32_t requestsize, void* reply, uint32_t replysize)
  {
    if (! lock_) { return EIO; }

    IOLockWrapper::ScopedLock lk(lock_);

    if (type == KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE) {
      if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_hide_statusmessage)) {
        if (! request) return 0;
        char* p = reinterpret_cast<KeyRemap4MacBook_bridge::StatusMessage::Request*>(request)->message;
        if (p[0] != '\0') {
          return 0;
        }
      }
    }

    // ------------------------------------------------------------
    int result = 0;
    int error = 0;
    socket_t socket;
    bool isMakeSocket = false;

    if (! makeSocket(socket)) {
      result = EIO;
      goto finish;
    }
    isMakeSocket = true;

    if (! connectSocket(socket)) {
      result = EIO;
      goto finish;
    }

    // ----------------------------------------
    // We can use static variable, because this function uses lock at the beginning.
    static struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    static struct iovec aiov[3];
    size_t iolen;
    aiov[0].iov_base = reinterpret_cast<caddr_t>(&type);
    aiov[0].iov_len = sizeof(type);
    if (requestsize <= 0) {
      msg.msg_iovlen = 1;
    } else {
      aiov[1].iov_base = reinterpret_cast<caddr_t>(&requestsize);
      aiov[1].iov_len = sizeof(requestsize);
      aiov[2].iov_base = reinterpret_cast<caddr_t>(request);
      aiov[2].iov_len = requestsize;
      msg.msg_iovlen = 3;
    }
    msg.msg_iov = aiov;

    error = sock_send(socket, &msg, 0, &iolen);
    if (error) {
      IOLOG_ERROR("Client::sendmsg sock_send failed error:%d\n", error);
      result = error;
      goto finish;
    }

    // ----------------------------------------
    if (replysize > 0) {
      memset(&msg, 0, sizeof(msg));

      uint32_t status = -1;
      aiov[0].iov_base = reinterpret_cast<caddr_t>(&status);
      aiov[0].iov_len = sizeof(status);
      aiov[1].iov_base = reinterpret_cast<caddr_t>(reply);
      aiov[1].iov_len = replysize;
      msg.msg_iov = aiov;
      msg.msg_iovlen = 2;

      error = sock_receive(socket, &msg, MSG_WAITALL, &iolen);
      if (error) {
        IOLOG_ERROR("Client::sendmsg sock_receive failed error:%d\n", error);
        result = error;
        goto finish;
      }
    }

  finish:
    if (isMakeSocket) {
      releaseSocket(socket);
    }
    if (result) {
      IOLOG_ERROR("Client::sendmsg error result:%d\n", result);
    }
    return result;
  }
}
