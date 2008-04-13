#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "bridge.hpp"

namespace KeyRemap4MacBook_client {
  enum {
    TIMEOUT = 1,
  };
  int sendmsg(KeyRemap4MacBook_bridge::RequestType type, void *request, size_t requestsize, void *reply, size_t replysize);
}

#endif
