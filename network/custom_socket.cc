#include "custom_socket.h"

CustomSocket::CustomSocket(): CustomSocketBase() {
  buflen_ = 1024;
  // delete ptr in destructor
  buf_ = new char[buflen_+1];
}

CustomSocket::~CustomSocket() {
  std::cout << "Destroying socket \n";
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
    if (nread < 0) {
      if (errno == EINTR) {
        std::cerr << "ERROR Interrupted: " << strerror(errno) << std::endl;
        retry++;
        if(retry > 10) {
          throw errno;
        } else {
          continue;
        }
      }
      else {
        std::cerr << "Error on write: " << strerror(errno) << std::endl;
        throw errno;
      }
    } else if (nread == 0) {
      std::cerr << "ERROR socket closed: " << strerror(errno) << std::endl;
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
    if ((nwritten = ::send(sockfd, ptr, nleft, 0)) < 0) {
      if (errno == EINTR) {
        std::cerr << "ERROR Interrupted: " << strerror(errno) << std::endl;
        retry++;
        if(retry > 10) {
          throw errno;
        } else {
          continue;
        }
      } else {
        std::cerr << "Error on write: " << strerror(errno) << std::endl;
        throw errno;
      }
    } else if (nwritten == 0) {
      std::cerr << "ERROR socket closed: " << strerror(errno) << std::endl;
      throw errno;
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
}
