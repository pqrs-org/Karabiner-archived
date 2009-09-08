#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "bridge.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace KeyRemap4MacBook_client {
    enum {
      TIMEOUT_SECOND = 0,
      TIMEOUT_MICROSECOND = 100 * 1000, // 100ms
    };
    void initialize(void);
    int sendmsg(KeyRemap4MacBook_bridge::RequestType type, void *request, size_t requestsize, void *reply, size_t replysize);
  }
}

#endif
