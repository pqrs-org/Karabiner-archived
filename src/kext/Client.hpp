#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "bridge.hpp"

namespace KeyRemap4MacBook_client {
  namespace Bridge {
    enum {
      BRIDGE_TIMEOUT = 1,
    };

    int sendmsg(KeyRemap4MacBook_server::Bridge::RequestType type, void *request, size_t requestsize, void *reply, size_t replysize);
  }
}

#endif
