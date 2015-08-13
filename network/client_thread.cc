#include "client_thread.h"

ClientThread::ClientThread(int client_socket, ClientHandler handler) {
  time_t rawtime;
  struct tm * timeinfo;

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
  time (&rawtime);
  timeinfo = gmtime (&rawtime);

  strftime(conn_time, 80, "%c UTC", timeinfo);
}

std::string ClientThread::get_client_ip_address() {
  char dest[len];
  inet_ntop(addr.sin_family, &addr.sin_addr, dest, len);

  return std::string(dest);
}

std::string ClientThread::get_client_conn_time() {
  return std::string(conn_time);
}

ClientThread::~ClientThread() {}
