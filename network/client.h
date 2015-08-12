#include "custom_socket.h"

class Client: public CustomSocket {
private:
  struct hostent *server;

public:
  Client(short family, std::string address, unsigned short port); 
};
