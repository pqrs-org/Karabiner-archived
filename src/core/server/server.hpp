#ifndef KEYREMAP4MACBOOK_SERVER_HPP
#define KEYREMAP4MACBOOK_SERVER_HPP

#include <queue>
#include "bridge.hpp"

namespace KeyRemap4MacBook_server {
  class Server {
  public:
    Server(void) : listenSocket(-1) {}
    void doLoop(void);

  private:
    bool initialize(void);
    void terminate(void);

    bool makeSocket(void);
    void deleteSocket(void);

    void sendReply(int sock, void *data, size_t size, int error);
    void dispatchOperator(int sock);

    org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error do_ActiveApplicationInfo(org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ActiveApplicationInfo::Reply *reply);

    int listenSocket;
  };
}

#endif
