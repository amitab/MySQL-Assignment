#pragma once

#include <cstring>
#include <string>
#include <errno.h>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

class CustomSocketBase {

protected:
  int sockfd;
  struct sockaddr_in addr;

  void shutdown() {
    ::shutdown(this->sockfd, SHUT_RDWR);
    ::close(this->sockfd);
  }

public:
  CustomSocketBase() {
    std::memset((void *) &addr, 0, sizeof(addr));
  }

  ~CustomSocketBase() {
  }

  bool create_socket() {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sockfd < 0) {
      std::cerr << "ERROR opening socket: " << strerror(errno) << std::endl;
    }
    return (this->sockfd >= 0);
  }

  bool bind_socket(short family, unsigned long address, unsigned short port) {
    this->addr.sin_family = family;
    this->addr.sin_addr.s_addr = htonl(address);
    this->addr.sin_port = htons(port);
    if (bind(this->sockfd, (struct sockaddr *) &(this->addr), sizeof(this->addr)) < 0) {
       std::cerr << "ERROR on binding: " << strerror(errno) << std::endl;
       return false;
    }
    return true;
  }

};
