#include "lazy_thread_pool.h"

void LazyThreadPool::create_worker(int idle_time) {
  Worker* worker = new Worker(&client_queue_mutex, &client_queue, idle_time);
  worker->setup_cleanup(&LazyThreadPool::thread_cleanup, (void*) this);
  worker->unique_id = workers.size();
  workers.push_back(worker);
  inactive_workers.push(worker->unique_id);
}

void LazyThreadPool::deactivate_worker(pthread_t id) {
  inactive_workers.push(active_workers[id]);
  active_workers.erase(id);
  dead_threads.push(id);
}

void LazyThreadPool::clear_dead_threads() {
  pthread_t id;
  while (!dead_threads.empty()) {
    id = dead_threads.front();
    dead_threads.pop();
    pthread_join(id, NULL);
  }
}

void LazyThreadPool::start_worker(int id) {
  workers[id]->start();
  active_workers[workers[id]->get_id()] = id;
}

void LazyThreadPool::worker_start_working() {
  if(client_queue_mutex.get_waiting_thread_count() == 0 && inactive_workers.size() > 0) {
    int id = inactive_workers.front();
    inactive_workers.pop();
    start_worker(id);
  }
}

std::string LazyThreadPool::get_client_list() {
  std::stringstream data;
  int i = 1;

  for (std::vector<Worker*>::iterator it = workers.begin() ; it != workers.end(); ++it) {
    if(!((*it)->is_active())) continue;

    data << i << ": " << (*it)->get_client_thread()->get_client_ip_address();
    data << ", " << (*it)->get_client_thread()->get_client_conn_time();
    if(it != (workers.end() - 1)) {
      data << "\n";
    }
    i++;
  }

  return data.str();
}

LazyThreadPool::LazyThreadPool(int size, int idle_time) {
  MAX_POOL_SIZE = size;
  thread_idle_time = idle_time;

  create_worker(0);

  for (int i = 0; i < size - 1; ++i) {
    create_worker(idle_time);
  }
}

bool LazyThreadPool::add_task(ClientThread* client_thread) {
  clear_dead_threads();
  std::cout << get_client_list() << std::endl;
  if(inactive_workers.size() == 0 && client_queue_mutex.get_waiting_thread_count() == 0) {
    client_thread->send_message("Server Full...");
    delete client_thread;
    return false;
  }
  std::cout << "MANAGER GETTING LOCK" << std::endl;
  client_queue_mutex.accquire_lock();
  std::cout << "MANAGER GOT LOCK" << std::endl;
  client_queue.push(client_thread);
  
  worker_start_working();
  client_thread->send_message("Welcome!");
  std::cout << "MANAGER SIGNALING NOT EMPTY" << std::endl;
  client_queue_mutex.signal_q_not_empty();
  std::cout << "MANAGER UNLOCKING" << std::endl;
  client_queue_mutex.unlock();
  std::cout << "MANAGER RELEASED LOCK" << std::endl;

  return true;
}

LazyThreadPool::~LazyThreadPool() {
  std::map<pthread_t, int>::iterator it;
  int worker_id;
  pthread_t thread_id;

  client_queue_mutex.broadcast_workers();
  while (!active_workers.empty()) {
    it = active_workers.begin();
    
    thread_id = it->first;
    worker_id = it->second;

    pthread_cancel(thread_id);
    workers[worker_id]->wait_for_exit();
  }
  active_workers.clear();
  
  for (std::vector<Worker*>::iterator it = workers.begin() ; it != workers.end(); ++it) {
    if((*it)->get_id() != 0) {
      pthread_cancel((*it)->get_id());
      workers[(*it)->unique_id]->wait_for_exit();
    }
    delete (*it);
  }
  clear_dead_threads();
  workers.clear();

  while (!client_queue.empty()) {
    ClientThread* t = client_queue.front();
    delete t;
    client_queue.pop();
  }
}

bool LazyThreadPool::has_active_clients() {
  for (std::vector<Worker*>::iterator it = workers.begin() ; it != workers.end(); ++it) {
    if((*it)->is_active()) {
      return true;
    }
  }
  return false;
}
