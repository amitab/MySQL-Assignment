#include "client_thread.h"

ClientThread::ClientThread(int client_socket, ClientHandler handler) {
  admin = false;
  len = sizeof(addr);
  if(getpeername(client_socket, (struct sockaddr *)&addr, &len) == -1) {
    throw errno;
  }
  sockfd = client_socket;

  if(sockfd < 0) {
    std::cerr << "ERROR on accept: " << strerror(errno) << std::endl;
    throw errno;
  }

  client_handler = handler;
}

std::string ClientThread::get_client_ip_address() {
  char* dest = new char[len];
  inet_ntop(addr.sin_family, &addr.sin_addr, dest, len);
  std::string ip = std::string(dest);
  delete[] dest;
  return ip;
}
