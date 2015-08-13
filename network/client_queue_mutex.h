#ifndef __CLIENTQUEUEMUTEX_H__
#define __CLIENTQUEUEMUTEX_H__

#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <errno.h>
#include <cstring>

class ClientQueueMutex {
  pthread_cond_t queue_not_empty;
  pthread_cond_t queue_access;

  volatile bool is_locked;
  volatile bool is_empty;

  pthread_mutex_t m_lock;
  pthread_mutexattr_t m_attr;
  int wait_count;

public:

  int get_waiting_thread_count();

  ClientQueueMutex();

  void accquire_lock();

  int try_to_lock_queue();

  void lock_wait_for_access();

  int wait_for_access();

  void signal_access();

  void set_empty();

  void signal_not_empty();

  void lock_access_queue_not_empty();

  int timed_lock_access_queue_not_empty(int seconds);

  void unlock();

  ~ClientQueueMutex();

  void broadcast_conds();
};

#endif
