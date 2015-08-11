#include <cstring>
#include <string>
#include <errno.h>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include "server.h"
Server* server;

void* handle_clients(ClientThread* client_thread) {
  // setup handler
  std::string write_buffer = "Got the message;";
  std::string read_buffer;
  while(1) {
    read_buffer = client_thread->recieve_message();
    std::cout << "Recieved: " << read_buffer << std::endl;

    if(read_buffer.compare("EXIT") == 0) {
      client_thread->send_message("Bye");
      break;
    } else if(read_buffer.compare("status") == 0) {
      client_thread->send_message(server->get_clients_connected());
    } else if(read_buffer.compare("admin") == 0) {
      client_thread->send_message("Enter Password: ");
      read_buffer = client_thread->recieve_message();
      if(read_buffer.compare("root") == 0) {
        client_thread->admin = true;
        client_thread->send_message("Authorized");
      } else {
        client_thread->send_message("Unauthorized");
      }
    } else if(read_buffer.compare("kill") == 0) {
      if(client_thread->admin) {
        client_thread->send_message("Shutting down server...");
        server->kill_server();
        break;
      } else {
        client_thread->send_message("Invalid command");
      }
    } else {
      client_thread->send_message(write_buffer);
    }
  }
}

int main(int argc, char *argv[]) {
  server = new Server(AF_INET, INADDR_ANY, atoi(argv[1]), 4, 10);
  server->serve(5, &handle_clients);

  delete server;
  pthread_exit(0);
}
