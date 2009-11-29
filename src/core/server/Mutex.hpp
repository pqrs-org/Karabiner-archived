#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <pthread.h>

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

#endif
