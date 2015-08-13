#include "custom_socket.h"

CustomSocket::CustomSocket(): CustomSocketBase() {
  buflen_ = 1024;
  // delete ptr in destructor
  buf_ = new char[buflen_+1];
}

CustomSocket::~CustomSocket() {
  delete[] buf_;
  shutdown();
}

std::string CustomSocket::recieve_message() {
  return recv(this->sockfd);
}

std::string CustomSocket::recieve_message(int sockfd) {
  return recv(sockfd);
}

void CustomSocket::send_message(std::string message) {
  send(message, this->sockfd);
}

void CustomSocket::send_message(std::string message, int sockfd) {
  send(message, sockfd);
}

std::string CustomSocket::recv(int sockfd) {
  std::string request = "";
  int retry = 0;
  while (request.find(";") == std::string::npos) {
    int nread = ::recv(sockfd,buf_,1024,0);
    if (nread <= 0) {
      throw errno;
    }
    request.append(buf_,nread);
  }
  return request.substr(0, request.find(";", 0));;
}

void CustomSocket::send(std::string response, int sockfd) {
  response += ";";
  int retry = 0;
  const char* ptr = response.c_str();
  int nleft = response.length();
  int nwritten;
  while (nleft) {
    if ((nwritten = ::send(sockfd, ptr, nleft, 0)) <= 0) {
      throw errno;
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
}
