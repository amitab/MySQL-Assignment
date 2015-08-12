#include "db/shared_ds.h"
#include "network/server.h"
#include "parser/client_parser.h"

Server* server;
SharedDS* shared_ds;

void* handle_clients(ClientThread* client_thread) {
  // setup handler
  ClientParser client_parser(server, shared_ds, client_thread);
  std::string read_buffer;

  while(1) {
    read_buffer = client_thread->recieve_message();
    std::cout << "Recieved: " << read_buffer << std::endl;

    if(read_buffer.compare("EXIT") == 0) {
      client_thread->send_message("Bye");
      break;
    } else if(read_buffer.compare("kill") == 0) {
      if(client_thread->admin) {
        client_thread->send_message("Shutting down server...");
        server->kill_server();
        break;
      } else {
        client_thread->send_message("Invalid command");
      }
    } else {
      client_thread->send_message(client_parser.parse_and_eval(read_buffer));
    }
  }
}

int main(int argc, char *argv[]) {
  int hash_size = atoi(argv[2]);
  shared_ds = new SharedDS(hash_size);
  server = new Server(AF_INET, INADDR_ANY, atoi(argv[1]), 4, 10);
  server->serve(5, &handle_clients);

  delete server;
  delete shared_ds;
  pthread_exit(0);
}
