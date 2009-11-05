#ifndef KEYREMAP4MACBOOK_SERVER_HPP
#define KEYREMAP4MACBOOK_SERVER_HPP

#include "bridge.hpp"
#include <string>

namespace KeyRemap4MacBook_server {
  class Server {
  public:
    Server(void) : listenSocket_(-1) {}
    bool initialize(const char* basedirectory);
    void doLoop(void);

  private:
    bool makeSocket(void);

    void sendReply(int sock, void *data, size_t size, int error);
    void dispatchOperator(int sock);

    org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error do_GetWorkspaceData(org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& reply);

    int listenSocket_;
    std::string socketpath_;
  };
}

#endif
