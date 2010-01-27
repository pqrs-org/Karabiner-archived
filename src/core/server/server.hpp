#ifndef KEYREMAP4MACBOOK_SERVER_HPP
#define KEYREMAP4MACBOOK_SERVER_HPP

#ifdef __cplusplus

#include "bridge.hpp"
#include <string>
#include <pthread.h>

namespace KeyRemap4MacBook_server {
  class Server {
  public:
    Server(void) : listenSocket_(-1) {}
    bool initialize(const char* basedirectory);
    void doLoop(void);
    const std::string getSocketPath(void) { return socketpath_; }

  private:
    bool makeSocket(void);

    void sendReply(int sock, void* data, size_t size, int error);
    void dispatchOperator(int sock);

    org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error do_GetWorkspaceData(org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& reply);

    int listenSocket_;
    std::string socketpath_;
  };
}
#endif

// ======================================================================
// for Objective-C
#ifdef __cplusplus
extern "C" {
#endif

void setCurrentApplicationType(const char* applicationName);
void setCurrentInputMode(const char* inputmodeName);

#ifdef __cplusplus
}
#endif

#endif
