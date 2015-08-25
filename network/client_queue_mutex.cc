#include "client_queue_mutex.h"

bool ClientQueueMutex::has_lock()  {
  if(pthread_self() == curr_thread) return true;
  return false;
}

int ClientQueueMutex::get_waiting_thread_count() {
  return wait_count;
}

ClientQueueMutex::ClientQueueMutex() {
  pthread_mutexattr_init(&m_attr);
  pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_ERRORCHECK);
  pthread_mutexattr_setrobust(&m_attr, PTHREAD_MUTEX_ROBUST);

  pthread_mutex_init(&m_lock, &m_attr);
  wait_count = 0;

  pthread_cond_init(&q_not_empty_cond, NULL);
  pthread_cond_init(&q_empty_cond, NULL);
  q_empty = true;
}

void ClientQueueMutex::accquire_lock() {
  wait_count++;
  int ret_val = pthread_mutex_lock(&m_lock);
  if(ret_val == 0) curr_thread = pthread_self();
  wait_count--;
}

int ClientQueueMutex::try_to_lock_queue() {
  return pthread_mutex_trylock(&m_lock);
}

void ClientQueueMutex::unlock() {
  int err = pthread_mutex_unlock(&m_lock);
  curr_thread = 0;
}

ClientQueueMutex::~ClientQueueMutex() {
  while(is_locked);
  unlock(); // Unlock Mutex after shared resource is safe
  pthread_mutex_destroy(&m_lock);

  pthread_cond_destroy(&queue_not_empty);
  pthread_cond_destroy(&queue_access);
}

void ClientQueueMutex::signal_q_empty() {
  q_empty = true;
  pthread_cond_signal(&q_empty_cond);
}

void ClientQueueMutex::signal_q_not_empty() {
  q_empty = false;
  pthread_cond_signal(&q_not_empty_cond);
}

int ClientQueueMutex::worker_lock() {
  wait_count++;
  int cond_val = 0;
  int ret_val = pthread_mutex_lock(&m_lock);

  while(q_empty && ret_val == 0) {
    cond_val = pthread_cond_wait(&q_not_empty_cond, &m_lock);
  }

  wait_count--;
  if(cond_val == 0) {curr_thread = pthread_self(); std::cout << "worker " << pthread_self() << " got lock" << std::endl;}

  return cond_val | ret_val;
}

int ClientQueueMutex::timed_worker_lock(int seconds) {
  wait_count++;
  struct timespec to = {0};
  clock_gettime(CLOCK_REALTIME, &to);
  to.tv_sec += seconds;

  int cond_val = 0;
  int ret_val = pthread_mutex_timedlock(&m_lock, &to);

  while(q_empty && ret_val == 0) {
    cond_val = pthread_cond_timedwait(&q_not_empty_cond, &m_lock, &to);
    if(cond_val == ETIMEDOUT) break;
  }

  wait_count--;
  if(cond_val == 0) {curr_thread = pthread_self(); std::cout << "worker " << pthread_self() << " got lock" << std::endl;}
  return cond_val | ret_val;
}

int ClientQueueMutex::manager_lock() {
  wait_count++;
  int cond_val = -1;
  int ret_val = pthread_mutex_lock(&m_lock);

  while(!q_empty && ret_val == 0) {
    cond_val = pthread_cond_wait(&q_empty_cond, &m_lock);
  }

  wait_count--;
  if(cond_val == 0) curr_thread = pthread_self();
  return cond_val;
}

void ClientQueueMutex::broadcast_workers() {
  pthread_cond_broadcast(&q_not_empty_cond);
}
