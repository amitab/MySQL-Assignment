#ifndef __LAZYTHREADPOOL_H__
#define __LAZYTHREADPOOL_H__

#include <map>
#include <queue>
#include <sstream>

#include "client_queue_mutex.h"
#include "worker.h"
#include "client_thread.h"

class LazyThreadPool {
  typedef std::queue<ClientThread*> ClientQueue;

  ClientQueue client_queue;
  ClientQueueMutex client_queue_mutex;

  std::queue<pthread_t> dead_threads;

  std::queue<int> inactive_workers;
  std::map<pthread_t, int> active_workers;
  std::vector<Worker*> workers;

  int MAX_POOL_SIZE;
  int thread_idle_time;

  void create_worker(int idle_time);

  void deactivate_worker(pthread_t id);

  void clear_dead_threads();

  void start_worker(int id);

  void worker_start_working();

public:

  static void thread_cleanup (void* args) {
    LazyThreadPool* self = (LazyThreadPool* ) args;
    pthread_t id = pthread_self();
    std::cout << id << ": Calling Thread cleanup!\n";

    self->deactivate_worker(id);
  }

  std::string get_client_list();

  LazyThreadPool(int size, int idle_time);

  bool add_task(ClientThread* client_thread);

  ~LazyThreadPool();
};

#endif
