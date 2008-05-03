#ifndef KEYREMAP4MACBOOK_SERVER_HPP
#define KEYREMAP4MACBOOK_SERVER_HPP

#include <queue>
#include "thread.hpp"
#include "bridge.hpp"

namespace KeyRemap4MacBook_server {
  class Server {
  public:
    enum {
      NUM_THREAD = 4,
    };
    Server(void) {
      listenSocket = -1;
    }

    void doLoop(void);

  private:
    bool initialize(void);
    void terminate(void);

    bool makeSocket(void);
    void deleteSocket(void);

    void sendReply(int sock, void *data, size_t size, int error);
    void dispatchOperator(int sock);
    static void *handleRequestThread(void *arg);
    bool enqueueRequest(int sock);

    KeyRemap4MacBook_bridge::Error do_ActiveApplicationInfo(KeyRemap4MacBook_bridge::ActiveApplicationInfo::Reply *reply);

    std::queue<int> queueRequestSocket;
    KeyRemap4MacBook_server::Thread::Mutex requestsMutex;
    KeyRemap4MacBook_server::Thread::Cond requestCond;

    int listenSocket;
    volatile bool exitLoopFlag;
  };
}

#endif
