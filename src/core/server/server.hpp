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
    bool process(void);
    const std::string getSocketPath(void) { return socketpath_; }

  private:
    bool makeSocket(void);

    void sendReply(int sock, void* data, size_t size, uint32_t error);
    void dispatchOperator(int sock);

    bool do_GetEssentialConfig(int sock);
    bool do_GetConfigCount(int sock);
    bool do_GetConfigInfo(int sock);
    bool do_GetConfigInitializeVector(int sock);

    int listenSocket_;
    std::string socketpath_;
  };
}
#endif

#endif
