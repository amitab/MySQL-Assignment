#ifndef __SERVER_H__
#define __SERVER_H__

#include "custom_socket_base.h"
#include "client_thread.h"
#include "lazy_thread_pool.h"

class Server: public CustomSocketBase {
// ClientHandler method gets client thread to communicate with client
typedef void* (* ClientHandler) (ClientThread* );

private:
  LazyThreadPool thread_manager;
  bool server_running;
  bool accept_client(ClientHandler handler);

public:
  Server(short family, unsigned long address, unsigned short port, int max_clients, int idle_time) : CustomSocketBase(), thread_manager(max_clients, idle_time) {
    create_socket();
    bind_socket(family, address, port);
    server_running = true;
  }
  
  std::string get_clients_connected();
  void serve(int backlog, ClientHandler handler);
  void kill_server();
  bool has_active_clients();
};

#endif
