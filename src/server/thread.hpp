#ifndef THREAD_HPP
#define THREAD_HPP

#include <pthread.h>

namespace KeyRemap4MacBook_server {
  namespace Thread {
    class Mutex {
    public:
      Mutex(void) {
        pthread_mutex_init(&mutex, NULL);
      }
      bool lock(void) {
        return pthread_mutex_lock(&mutex) == 0;
      }
      bool unlock(void) {
        return pthread_mutex_unlock(&mutex) == 0;
      }
      pthread_mutex_t *getRaw(void) { return &mutex; };

    private:
      pthread_mutex_t mutex;
    };

    class Cond {
    public:
      Cond(void) {
        pthread_cond_init(&cond, NULL);
      }
      bool wait(Mutex *mutex) {
        return pthread_cond_wait(&cond, mutex->getRaw()) == 0;
      }
      bool signal(void) {
        return pthread_cond_signal(&cond);
      }

    private:
      pthread_cond_t cond;
    };
  }
}

#endif
