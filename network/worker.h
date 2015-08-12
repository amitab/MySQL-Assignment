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

    try {
      self = (Worker*) args;
      std::cout << "New thread spawned: " << self->thread << "\n";
      pthread_cleanup_push(self->thread_cleanup_routine, self->thread_cleanup_args);

      while(true) {

        std::cout << "Waiting: " << self->thread << "\n";
        if(self->persist) {
          self->queue_access->lock_access_queue_not_empty();
        } else {
          if(self->queue_access->timed_lock_access_queue_not_empty(self->idle_time) != 0) {
            std::cout << "Thread Didnt get lock on time: " << self->thread << "\n\n";
            self->queue_access->unlock();
            break;
          }
        }

        std::cout << "Got lock: " << self->thread << "\n";

        std::cout << "Picking up a task: " << self->thread << "\n";
        self->active = true;

        std::cout << "Removed task from queue: " << self->thread << "\n";
        if(self->client_queue->size() > 0) {
          self->client_thread = self->client_queue->front();
          self->client_queue->pop();
        } else {
          self->client_thread = NULL;
        }

        std::cout << "Signaling: " << self->thread << "\n";
        self->queue_access->signal_access();
        if(self->client_queue->size() > 0) {
          std::cout << "Signaling threads not empty" << self->thread << std::endl;
          self->queue_access->signal_not_empty();
        } else {
          self->queue_access->set_empty();
          std::cout << "Signaling threads empty" << self->thread << std::endl;
        }

        std::cout << "Unlocking: " << self->thread << "\n";
        self->queue_access->unlock();

        if(self->client_thread != NULL) {
          std::cout << "Starting task: " << self->thread << "\n";
          self->client_thread->client_handler(self->client_thread);
        }

        self->active = false;
        std::cout << "Done with task: " << self->thread << "\n\n";
        self->delete_client_thread();
      }

      pthread_cleanup_pop(1);
    } catch (abi::__forced_unwind&) {
      self->the_situation();
      throw;
    } catch(int err) {
      self->the_situation();
      std::cerr << "Thread with id " << self->thread << " has stopped running: " << strerror(err) << "\n\n";
    }

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
