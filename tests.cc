#include "network/client.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {
  high_resolution_clock::time_point t1;
  high_resolution_clock::time_point t2;
  long duration;

  int portno;

  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " [host] [port]" << std::endl;
    exit(0);
  }
  portno = atoi(argv[2]);
  Client client(AF_INET, argv[1], portno);
  
  std::string read_message = client.recieve_message();
  std::cout << read_message << std::endl;

  std::stringstream query;

  t1 = high_resolution_clock::now();
  for (int i = 0; i < 10000; ++i) {
    query << "INSERT " << i;
    client.send_message(query.str());
    client.recieve_message();
    query.str("");
    query.clear();
  }
  t2 = high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  cout << "10000 insert time: " << duration << endl;


  t1 = high_resolution_clock::now();
  for (int i = 0; i < 10000; ++i) {
    query << "FIND " << i;
    client.send_message(query.str());
    client.recieve_message();
    query.str("");
    query.clear();
  }
  t2 = high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  cout << "10000 search time: " << duration << endl;

  return 0;
}
