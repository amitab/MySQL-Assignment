#include "worker.h"

void Worker::the_situation() {
  if(queue_access->has_lock()) {
    if(client_queue->size() > 0) {
      queue_access->signal_q_not_empty();
    } else {
      queue_access->signal_q_empty();
    }
    queue_access->unlock();
  }
  
  thread = 0;
  active = false;
  delete_client_thread();
}

Worker::Worker() {}

Worker::Worker(ClientQueueMutex* queue_access, ClientQueue* client_queue, int idle_time) {
  this->queue_access = queue_access;
  this->client_queue = client_queue;
  this->idle_time = idle_time;
  this->client_thread = NULL;
  this->thread = 0;

  if(idle_time == 0) {
    persist = true;
  } else {
    persist = false;
  }

  active = false;
  thread_cleanup_routine = NULL;
}

ClientThread* Worker::get_client_thread() {
  return client_thread;
}

bool Worker::start() {
  return (pthread_create(&thread, NULL, thread_entry_func, this) == 0);
}

bool Worker::is_active() {
  return active;
}

void Worker::delete_client_thread() {
  if(client_thread != NULL) {
    delete client_thread;
  }
  client_thread = NULL;
}

void Worker::setup_cleanup(void (*thread_cleanup_routine)(void*), void* args) {
  this->thread_cleanup_routine = thread_cleanup_routine;
  this->thread_cleanup_args = args;
}

Worker::~Worker() {
  delete_client_thread();
}

void Worker::wait_for_exit() {
  (void) pthread_join(thread, NULL);
}

pthread_t& Worker::get_id() {
  return thread;
}
