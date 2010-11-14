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

    void sendReply(int sock, void* data, size_t size, uint32_t error);
    void dispatchOperator(int sock);

    bool do_GetEssentialConfig(int sock);
    bool do_GetConfigCount(int sock);
    bool do_GetConfigInfo(int sock);
    bool do_GetConfigInitializeVector(int sock);
    bool do_GetWorkspaceData(int sock);
    org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error do_ChangeInputMode(const org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::ChangeInputMode::Request& request);
    org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error do_StatusMessage(const org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessage::Request& request);
    org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::Error do_StatusMessageWindowParameter(const org_pqrs_KeyRemap4MacBook::KeyRemap4MacBook_bridge::StatusMessageWindowParameter::Request& request);

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

void setCurrentApplicationType(unsigned int newval);
void setCurrentInputMode(unsigned int inputmode, unsigned int inputmodedetail);

#ifdef __cplusplus
}
#endif

#endif
