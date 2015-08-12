#ifndef __CLIENTTHREAD_H__
#define __CLIENTTHREAD_H__

#include "custom_socket.h"

class ClientThread: public CustomSocket {
// ClientHandler method gets client thread to communicate with client
typedef void* (* ClientHandler) (ClientThread* );

public:
  ClientHandler client_handler;
  socklen_t len;
  bool admin;

  ClientThread(int server_socket, ClientHandler handler);

  std::string get_client_ip_address();

};

#endif
