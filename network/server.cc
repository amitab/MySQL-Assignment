#include "server.h"

bool Server::accept_client(ClientHandler handler) {
  // This ptr deleted in thread_manager
  ClientThread* client_thread = new ClientThread(sockfd, handler);
  return thread_manager.add_task(client_thread);
}

std::string Server::get_clients_connected() {
  return thread_manager.get_client_list();
}

void Server::serve(int backlog, ClientHandler handler) {
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

void Server::kill_server() {
  server_running = false;
  shutdown();
}
