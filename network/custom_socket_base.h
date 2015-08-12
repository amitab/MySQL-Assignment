#ifndef __CUSTOMSOCKETBASE_H__
#define __CUSTOMSOCKETBASE_H__

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

  void shutdown();
  int accept_connection(int server_socket, struct sockaddr_in& addr);
public:
  CustomSocketBase();

  ~CustomSocketBase();

  bool create_socket();

  bool bind_socket(short family, unsigned long address, unsigned short port);

};

#endif
