#include <pthread.h>
#include <time.h>
#include <sys/time.h>

class AVLMutex {
  pthread_cond_t avl_access;
  volatile bool is_locked;

  pthread_mutex_t avl_lock;
  pthread_mutexattr_t avl_attr;

  public:
  AVLMutex() {
    pthread_cond_init(&avl_access, NULL);
    is_locked = false;

    pthread_mutexattr_init(&avl_attr);
    pthread_mutexattr_settype(&avl_attr, PTHREAD_MUTEX_ERRORCHECK);

    pthread_mutex_init(&avl_lock, &avl_attr);
  }

  void accquire_lock() {
    pthread_mutex_lock(&avl_lock);
    while (is_locked) {
      pthread_cond_wait(&avl_access, &avl_lock);
    }
    is_locked = true;
  }

  void unlock() {
    pthread_mutex_unlock(&avl_lock);
  }

  void signal_free() {
    pthread_cond_signal(&avl_access);
    is_locked = false;
  }
};
