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

  volatile bool q_empty;
  pthread_cond_t q_empty_cond;
  pthread_cond_t q_not_empty_cond;
  pthread_t curr_thread;

  pthread_mutex_t m_lock;
  pthread_mutexattr_t m_attr;
  int wait_count;

public:

  bool has_lock();

  int get_waiting_thread_count();

  ClientQueueMutex();

  void accquire_lock();

  int try_to_lock_queue();

  void unlock();

  ~ClientQueueMutex();

  void signal_q_empty();

  void signal_q_not_empty();

  int worker_lock();

  int timed_worker_lock(int seconds);

  int manager_lock();

  void broadcast_workers();
};

#endif
