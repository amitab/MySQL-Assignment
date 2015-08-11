#include <readline/readline.h>
#include <readline/history.h>

#include "client.h"

int main(int argc, char *argv[]) {
  int portno;

  char* write_buffer = new char[1024];
  std::string read_message = "";
  std::string write_message = "";

  if (argc <3) {
    std::cout << "Usage: " << argv[0] << " host port" << std::endl;
    exit(0);
  }
  portno = atoi(argv[2]);
  Client client(AF_INET, argv[1], portno);

  read_message = client.recieve_message();
  std::cout << read_message << std::endl;

  while(1) {
    std::cout << "Enter Message: ";
    write_buffer = readline("");
    write_message.append(write_buffer, strlen(write_buffer));

    client.send_message(write_message);
    read_message = client.recieve_message();

    std::cout << read_message << std::endl;
    if(write_message.compare("EXIT") == 0) break;
    write_message = "";
  }

  delete write_buffer;
  return 0;
}
