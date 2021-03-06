#include <signal.h>
#include <unistd.h>

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

void signal_handler(int signum) {
  signal(SIGINT, signal_handler);

  if(server->has_active_clients()) {
    std::cout << "Cannot shutdown! Clients connected!" << std::endl;
  } else {
    cout << "Force Shutting down server!" << endl;
    server->kill_server();

    delete server;
    delete shared_ds;
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  if(argc < 4) {
    cout << "USAGE: " << argv[0] << " [port] [hash_size] [worker_count] [idle_time]" << endl;
    return 0;
  }

  signal(SIGINT, signal_handler);

  int hash_size = atoi(argv[2]);
  int worker_count = atoi(argv[3]);
  int idle_time = atoi(argv[4]);
  int port = atoi(argv[1]);

  shared_ds = new SharedDS(hash_size);
  server = new Server(AF_INET, INADDR_ANY, port, worker_count, idle_time);
  server->serve(5, &handle_clients);

  delete server;
  delete shared_ds;
  pthread_exit(0);
}
