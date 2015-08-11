#include "db/shared_ds.cc"
#include "parser/client_parser.cc"
#include "network/server.h"
Server* server;
SharedDS* shared_ds;
ClientParser* client_parser;

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
  int hash_size = atoi(argv[2]);
  shared_ds = new SharedDS(hash_size);
  server = new Server(AF_INET, INADDR_ANY, atoi(argv[1]), 4, 10);
  client_parser = new ClientParser(server, shared_ds);
  server->serve(5, &handle_clients);

  delete server;
  delete shared_ds;
  delete client_parser;
  pthread_exit(0);
}
