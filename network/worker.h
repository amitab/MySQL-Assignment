#ifndef __WORKER_H__
#define __WORKER_H__

#include <cstring>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <queue>
#include <ctime>

#include <pthread.h>
#include <errno.h>
#include "client_thread.h"
#include "client_queue_mutex.h"
#include <cxxabi.h>


class Worker {
  typedef std::queue<ClientThread*> ClientQueue;

  int idle_time;
  time_t thread_start_time;
  pthread_t thread;
  bool persist;

  // Dont delete these. You dont have ownership.
  ClientQueueMutex* queue_access;
  ClientQueue* client_queue;
  void* thread_cleanup_args;
  // Delete this ptr at end of every thread loop
  ClientThread* client_thread;

  void (*thread_cleanup_routine)(void*);
  bool active;

  void the_situation();

  static void * thread_entry_func(void * args) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_t id = pthread_self();
    Worker* self;
    std::cout << "Thread born: " << id << std::endl;
    try {
      self = (Worker*) args;
      pthread_cleanup_push(self->thread_cleanup_routine, self->thread_cleanup_args);

      while(true) {
        if(self->persist) {
          if(self->queue_access->worker_lock() != 0) {
            std::cout << pthread_self() << " Couldnt get lock! Trying again" << std::endl;
            continue;
          }
        } else {
          if(self->queue_access->timed_worker_lock(self->idle_time) != 0) {
            std::cout << "Thread Didnt get lock on time: " << self->thread << "\n\n";
            self->queue_access->unlock();
            break;
          }
        }

        self->active = true;
        if(self->client_queue->size() > 0) {
          self->client_thread = self->client_queue->front();
          self->client_queue->pop();
        } else {
          self->client_thread = NULL;
        }

        if(self->client_queue->size() > 0) {
          self->queue_access->signal_q_not_empty();
        } else {
          self->queue_access->signal_q_empty();
        }

        self->queue_access->unlock();

        if(self->client_thread != NULL) {
          self->client_thread->client_handler(self->client_thread);
        } else {
          std::cout << "Woke up and there was nothing! Killing myself." << std::endl;
          break;
        }

        self->active = false;
        self->delete_client_thread();
      }

      pthread_cleanup_pop(1);
    } catch (abi::__forced_unwind&) {
      self->the_situation();
      throw;
    } catch(int err) {
      self->the_situation();
      pthread_exit(0);
    }
    self->thread = 0;
    pthread_exit(0);
  }


  public:
  int unique_id;

  Worker();

  Worker(ClientQueueMutex* queue_access, ClientQueue* client_queue, int idle_time);

  ClientThread* get_client_thread();

  bool start();

  bool is_active();

  void delete_client_thread();

  void setup_cleanup(void (*thread_cleanup_routine)(void*), void* args);

  ~Worker();

  void wait_for_exit();

  pthread_t& get_id();
};

#endif
