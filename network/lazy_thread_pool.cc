#include <map>
#include <queue>
#include <sstream>

#include "client_queue_mutex.cc"
#include "worker.h"
#include "client_thread.cc"

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

  void create_worker(int idle_time) {
    Worker* worker = new Worker(&client_queue_mutex, &client_queue, idle_time);
    worker->setup_cleanup(&LazyThreadPool::thread_cleanup, (void*) this);
    worker->unique_id = workers.size();
    workers.push_back(worker);
    inactive_workers.push(worker->unique_id);
  }

  void deactivate_worker(pthread_t id) {
    inactive_workers.push(active_workers[id]);
    active_workers.erase(id);
    dead_threads.push(id);
  }

  void clear_dead_threads() {
    pthread_t id;
    while (!dead_threads.empty()) {
      id = dead_threads.front();
      dead_threads.pop();
      pthread_join(id, NULL);
    }
  }

  void start_worker(int id) {
    workers[id]->start();
    active_workers[workers[id]->get_id()] = id;
  }

  void worker_start_working() {
    if(client_queue_mutex.get_waiting_thread_count() == 0 && inactive_workers.size() > 0) {
      int id = inactive_workers.front();
      inactive_workers.pop();
      start_worker(id);
    }
  }

public:
  static void thread_cleanup (void* args) {
    LazyThreadPool* self = (LazyThreadPool* ) args;
    pthread_t id = pthread_self();
    std::cout << id << ": Calling Thread cleanup!\n";

    self->deactivate_worker(id);
  }

  std::string get_client_list() {
    std::stringstream data;
    int i = 1;

    for (std::vector<Worker*>::iterator it = workers.begin() ; it != workers.end(); ++it) {
      if(!((*it)->is_active())) continue;

      data << i << ": " << (*it)->get_client_thread()->get_client_ip_address();
      if(it != (workers.end() - 1)) {
        data << "\n";
      }
      i++;
    }

    return data.str();
  }

  LazyThreadPool(int size, int idle_time) {
    std::cout << "Init Threadpool\n";

    MAX_POOL_SIZE = size;
    thread_idle_time = idle_time;
    client_queue_mutex.accquire_lock();

    // Create one persistent thread
    create_worker(0);

    for (int i = 0; i < size - 1; ++i) {
      create_worker(idle_time);
    }
  }

  bool add_task(ClientThread* client_thread) {
    clear_dead_threads();
    std::cout << get_client_list() << std::endl;
    // Dont accept if pool full!
    if(inactive_workers.size() == 0 && client_queue_mutex.get_waiting_thread_count() == 0) {
      client_thread->send_message("Server Full...");
      delete client_thread;
      return false;
    }

    std::cout << "Waiting for queue access" << std::endl;
    client_queue_mutex.lock_wait_for_access();
    std::cout << "Got queue access" << std::endl;

    std::cout << "Adding task" << std::endl;
    client_queue.push(client_thread);

    std::cout << "Waiting threads: " << client_queue_mutex.get_waiting_thread_count() << std::endl;
    worker_start_working();

    client_thread->send_message("Welcome!");

    std::cout << "Signaling threads free for access" << std::endl;
    client_queue_mutex.signal_access();
    std::cout << "Signaling threads not empty" << std::endl;
    client_queue_mutex.signal_not_empty();
    std::cout << "Unlocking Mutex\n\n";
    client_queue_mutex.unlock();

    return true;
  }

  ~LazyThreadPool() {
    std::cout << "Killing threads in pool" << std::endl;
    std::map<pthread_t, int>::iterator it;
    while (!active_workers.empty()) {
      it = active_workers.begin();
      pthread_cancel(it->first);
      workers[it->second]->wait_for_exit();
    }
    active_workers.clear();
    for (std::vector<Worker*>::iterator it = workers.begin() ; it != workers.end(); ++it) {
      delete (*it);
    }
    clear_dead_threads();
    workers.clear();
  }
};
