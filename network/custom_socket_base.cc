#include "custom_socket_base.h"

void CustomSocketBase::shutdown() {
  ::shutdown(this->sockfd, SHUT_RDWR);
  ::close(this->sockfd);
}

CustomSocketBase::CustomSocketBase() {
  std::memset((void *) &addr, 0, sizeof(addr));
}

CustomSocketBase::~CustomSocketBase() {
}

bool CustomSocketBase::create_socket() {
  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(this->sockfd < 0) {
    std::cerr << "ERROR opening socket: " << strerror(errno) << std::endl;
  }
  return (this->sockfd >= 0);
}

bool CustomSocketBase::bind_socket(short family, unsigned long address, unsigned short port) {
  this->addr.sin_family = family;
  this->addr.sin_addr.s_addr = htonl(address);
  this->addr.sin_port = htons(port);
  if (bind(this->sockfd, (struct sockaddr *) &(this->addr), sizeof(this->addr)) < 0) {
    std::cerr << "ERROR on binding: " << strerror(errno) << std::endl;
    return false;
  }
  return true;
}


