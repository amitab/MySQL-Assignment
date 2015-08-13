#include "client.h"

Client::Client(short family, std::string address, unsigned short port): CustomSocket() {
    create_socket();
    server = gethostbyname(address.c_str());
    if (server == NULL) {
      throw errno;
    }

    addr.sin_family = AF_INET;
    std::memcpy(&addr.sin_addr, server->h_addr, server->h_length);
    addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
      throw errno;
    }
}

int Client::reconnect() {
  return connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
}

