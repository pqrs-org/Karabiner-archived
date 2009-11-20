#ifndef KEYREMAP4MACBOOK_SERVER_HPP
#define KEYREMAP4MACBOOK_SERVER_HPP

#ifdef __cplusplus

#include "bridge.hpp"
#include <string>
#include <pthread.h>

namespace KeyRemap4MacBook_server {
  class Mutex {
  public:
    Mutex(void) {
      pthread_mutex_init(&mutex_, NULL);
    }
    ~Mutex(void) {
      pthread_mutex_destroy(&mutex_);
    }
    int lock(void) {
      return pthread_mutex_lock(&mutex_);
    }
    int unlock(void) {
      return pthread_mutex_unlock(&mutex_);
    }

    class ScopedLock {
    public:
      ScopedLock(Mutex& m) : mutex_(m) {
        mutex_.lock();
      }
      ~ScopedLock(void) {
        mutex_.unlock();
      }
    private:
      Mutex& mutex_;
    };

  private:
    pthread_mutex_t mutex_;
  };

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

// ======================================================================
// for Objective-C
#ifdef __cplusplus
extern "C" {
#endif

void setCurrentApplicationType(const char* applicationName);

#ifdef __cplusplus
}
#endif

#endif
