#include "custom_socket.cc"

class Client: public CustomSocket {
private:
  struct hostent *server;

public:
  Client(short family, std::string address, unsigned short port): CustomSocket() {
    create_socket();
    server = gethostbyname(address.c_str());
    if (server == NULL) {
      std::cerr << "ERROR bad host" << strerror(errno) << std::endl;
      throw errno;
    }

    addr.sin_family = AF_INET;
    std::memcpy(&addr.sin_addr, server->h_addr, server->h_length);
    addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
      std::cerr << "Error connecting: " << strerror(errno) << std::endl;
      throw errno;
    }
  }
};
