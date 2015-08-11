#include "custom_socket_base.h"
#include "client_thread.cc"
#include "lazy_thread_pool.cc"

class Server: public CustomSocketBase {
// ClientHandler method gets client thread to communicate with client
typedef void* (* ClientHandler) (ClientThread* );

private:
  LazyThreadPool thread_manager;
  bool server_running;

  bool accept_client(ClientHandler handler) {
    // This ptr deleted in thread_manager
    ClientThread* client_thread = new ClientThread(sockfd, handler);
    return thread_manager.add_task(client_thread);
  }

public:
  Server(short family, unsigned long address, unsigned short port, int max_clients, int idle_time) : CustomSocketBase(), thread_manager(max_clients, idle_time) {
    create_socket();
    bind_socket(family, address, port);
    server_running = true;
  }

  std::string get_clients_connected() {
    return thread_manager.get_client_list();
  }

  void serve(int backlog, ClientHandler handler) {
    listen(this->sockfd, backlog);
    while(server_running) {
      try {
        accept_client(handler);
      } catch (int err) {
        std::cout << "Error while client accept: " << strerror(err) << std::endl;
      }
    }
    std::cout << "Stopped serving more clients." << std::endl;
  }

  void kill_server() {
    server_running = false;
    shutdown();
  }

};
