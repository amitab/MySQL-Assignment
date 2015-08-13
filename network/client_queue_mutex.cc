#include "client_queue_mutex.h"

int ClientQueueMutex::get_waiting_thread_count() {
  return wait_count;
}

ClientQueueMutex::ClientQueueMutex() {
  pthread_cond_init(&queue_not_empty, NULL);
  pthread_cond_init(&queue_access, NULL);
  is_empty = true;
  is_locked = false;

  pthread_mutexattr_init(&m_attr);
  pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_ERRORCHECK);

  pthread_mutex_init(&m_lock, &m_attr);
  wait_count = 0;
}

void ClientQueueMutex::accquire_lock() {
  wait_count++;
  int ret_val = pthread_mutex_lock(&m_lock);
  wait_count--;
}

int ClientQueueMutex::try_to_lock_queue() {
  return pthread_mutex_trylock(&m_lock);
}

void ClientQueueMutex::lock_wait_for_access() {
  wait_count++;
  int ret_val = pthread_mutex_lock(&m_lock);

  while(is_locked) {
    int ret_val = pthread_cond_wait(&queue_access, &m_lock);
  }
  is_locked = true;

  wait_count--;
}

int ClientQueueMutex::wait_for_access() {
  wait_count++;
  int ret_val;

  while(is_locked) {
    ret_val = pthread_cond_wait(&queue_access, &m_lock);
  }

  is_locked = true;
  wait_count--;
  return ret_val;
}

void ClientQueueMutex::signal_access() {
  pthread_cond_signal(&queue_access);
  is_locked = false;
}

void ClientQueueMutex::set_empty() {
  is_empty = true;
}

void ClientQueueMutex::signal_not_empty() {
  pthread_cond_signal(&queue_not_empty);
  is_empty = false;
}

void ClientQueueMutex::lock_access_queue_not_empty() {
  wait_count++;
  int ret_val = pthread_mutex_lock(&m_lock);

  while(is_empty) {
    int ret_val = pthread_cond_wait(&queue_not_empty, &m_lock);
  }

  while(is_locked) {
    int ret_val = pthread_cond_wait(&queue_access, &m_lock);
  }
  is_locked = true;
  wait_count--;
}

int ClientQueueMutex::timed_lock_access_queue_not_empty(int seconds) {
  wait_count++;
  struct timespec to = {0};
  clock_gettime(CLOCK_REALTIME, &to);
  to.tv_sec += seconds;

  int ret_val = pthread_mutex_timedlock(&m_lock, &to);
  if (ret_val != 0) {
    pthread_mutex_unlock(&m_lock);
    wait_count--;
    return ret_val;
  }

  while (is_empty) {
    ret_val = pthread_cond_timedwait(&queue_not_empty, &m_lock, &to);
    if (ret_val != 0) {
      wait_count--;
      return ret_val;
    }
  }

  while (is_locked) {
    ret_val = pthread_cond_timedwait(&queue_access, &m_lock, &to);
    if (ret_val != 0) {
      wait_count--;
      return ret_val;
    }
  }

  is_locked = true;
  wait_count--;
  return 0;
}

void ClientQueueMutex::unlock() {
  int err = pthread_mutex_unlock(&m_lock);
}

ClientQueueMutex::~ClientQueueMutex() {
  while(is_locked);
  unlock(); // Unlock Mutex after shared resource is safe
  pthread_mutex_destroy(&m_lock);

  pthread_cond_destroy(&queue_not_empty);
  pthread_cond_destroy(&queue_access);
}

void ClientQueueMutex::broadcast_conds() {
  is_empty = false;
  pthread_cond_broadcast(&queue_not_empty);
  is_locked = false;
  pthread_cond_broadcast(&queue_access);
}
