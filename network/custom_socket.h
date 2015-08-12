#ifndef __CUSTOMSOCKET_H__
#define __CUSTOMSOCKET_H__

#include "custom_socket_base.h"

class CustomSocket: public CustomSocketBase {

protected:
  int buflen_;
  char* buf_;

public:
  CustomSocket();

  ~CustomSocket();

  virtual std::string recieve_message();

  virtual std::string recieve_message(int sockfd);

  virtual void send_message(std::string message);

  virtual void send_message(std::string message, int sockfd);

  std::string recv(int sockfd);

  void send(std::string response, int sockfd);

};

#endif
