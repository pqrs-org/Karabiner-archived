extern "C" {
#include <sys/systm.h>
#include <sys/un.h>
#include <sys/kpi_socket.h>
  errno_t sock_nointerrupt(socket_t so, int on);
}
#include "Client.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    struct sockaddr_un sockaddr_;

    void
    releaseSocket(socket_t &socket)
    {
      sock_shutdown(socket, SHUT_RDWR);
      sock_close(socket);
    }

    bool
    makeSocket(socket_t &socket)
    {
      int error = sock_socket(PF_LOCAL, SOCK_STREAM, 0, NULL, NULL, &socket);
      if (error) {
        printf("[KeyRemap4MacBook ERROR] sock_socket failed(%d)\n", error);
        return false;
      }

      // ----------------------------------------
      struct timeval tv;
      tv.tv_sec = KeyRemap4MacBook_client::TIMEOUT_SECOND;
      tv.tv_usec = KeyRemap4MacBook_client::TIMEOUT_MICROSECOND;

      error = sock_setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
      if (error) {
        printf("[KeyRemap4MacBook ERROR] sock_setsockopt failed(%d)\n", error);
        goto error;
      }

      error = sock_setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval));
      if (error) {
        printf("[KeyRemap4MacBook ERROR] sock_setsockopt failed(%d)\n", error);
        goto error;
      }

      return true;

    error:
      releaseSocket(socket);
      printf("KeyRemap4MacBook_client makeSocket failed(%d)\n", error);

      return false;
    }

    bool
    connectSocket(socket_t &socket)
    {
      errno_t error = sock_connect(socket, reinterpret_cast<const sockaddr *>(&sockaddr_), 0);
      if (error) {
#if 0
        // the connection failure is no problem because a server does not start at login window.
        printf("[KeyRemap4MacBook ERROR] sock_connect failed(%d)\n", error);
#endif
        return false;
      }
      error = sock_nointerrupt(socket, TRUE);
      if (error) {
        printf("[KeyRemap4MacBook ERROR] sock_nointerrupt failed(%d)\n", error);
        return false;
      }

      return true;
    }
  }

  void
  KeyRemap4MacBook_client::initialize(void)
  {
    memset(&sockaddr_, 0, sizeof(sockaddr_));
    sockaddr_.sun_len = sizeof(sockaddr_);
    sockaddr_.sun_family = AF_UNIX;
    strncpy(sockaddr_.sun_path, KeyRemap4MacBook_bridge::socketPath, sizeof(sockaddr_.sun_path) - 8);
  }

  int
  KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::RequestType type, void *request, size_t requestsize, void *reply, size_t replysize)
  {
    socket_t socket;
    if (! makeSocket(socket)) {
      return EIO;
    }

    if (! connectSocket(socket)) {
      releaseSocket(socket);
      return EIO;
    }

    // ----------------------------------------
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    struct iovec aiov[3];
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

    int error = sock_send(socket, &msg, 0, &iolen);
    if (error) {
      printf("KeyRemap4MacBook_client::sendmsg sock_send failed(%d)\n", error);
      releaseSocket(socket);
      return error;
    }

    // ----------------------------------------
    memset(&msg, 0, sizeof(msg));

    int result = -1;
    aiov[0].iov_base = reinterpret_cast<caddr_t>(&result);
    aiov[0].iov_len = sizeof(result);
    aiov[1].iov_base = reinterpret_cast<caddr_t>(reply);
    aiov[1].iov_len = replysize;
    msg.msg_iov = aiov;
    msg.msg_iovlen = (replysize == 0 ? 1 : 2);

    error = sock_receive(socket, &msg, MSG_WAITALL, &iolen);
    if (error) {
      printf("KeyRemap4MacBook_client::sendmsg sock_receive failed(%d)\n", error);
    }

    releaseSocket(socket);
    if (error) {
      return error;
    }

    if (result) {
      printf("KeyRemap4MacBook_client::sendmsg error result (%d)\n", result);
    }
    return result;
  }
}
