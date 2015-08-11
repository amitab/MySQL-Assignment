#pragma once

#include "custom_socket.cc"


class ClientThread: public CustomSocket {
// ClientHandler method gets client thread to communicate with client
typedef void* (* ClientHandler) (ClientThread* );

public:
  ClientHandler client_handler;
  socklen_t len;
  bool admin;

  ClientThread(int server_socket, ClientHandler handler) {
    admin = false;
    len = sizeof(addr);
    sockfd = accept(server_socket, (struct sockaddr *)&addr, &len);

    if(sockfd < 0) {
      std::cerr << "ERROR on accept: " << strerror(errno) << std::endl;
      throw errno;
    }

    client_handler = handler;
  }

  std::string get_client_ip_address() {
    char* dest = new char[len];
    inet_ntop(addr.sin_family, &addr.sin_addr, dest, len);
    std::string ip = std::string(dest);
    delete[] dest;
    return ip;
  }

};
