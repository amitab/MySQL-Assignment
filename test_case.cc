#include "network/client.h"
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

int portno;
std::string address;
bool time_up;
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1,3);
std::uniform_int_distribution<int> number(1,2000000);

void print_msg(std::string msg) {
  std::cout << "[" << pthread_self()  << "] " << msg << std::endl;
}

std::string get_random_task() {
  std::stringstream query;
  int choice = distribution(generator);
  int data = number(generator);
  switch (choice) {
    case 1:
      query << "INSERT ";
      break;
    case 2:
      query << "DELETE ";
      break;
    case 3:
      query << "FIND ";
      break;
  }
  query << data;
  return query.str();
}

void* task(void* args) {
  Client client(AF_INET, address, portno);
  client.recieve_message();
  std::string query;

  while(!time_up) {
    query = get_random_task();
    print_msg(query);
    client.send_message(query);
    client.recieve_message();
    sleep(2);
  }

}

int main(int argc, char** argv) {
  portno = atoi(argv[2]);
  address = std::string(argv[1]);
  
  time_up = false;
  pthread_t id;

  for (int i = 0; i < 5; ++i) {
    pthread_create(&id, NULL, task, NULL);
    std::cout << "[" << id << "] CREATED" << std::endl;
    pthread_detach(id);
  }

  sleep(30 * 60);
  time_up = true;

  

  exit(0);
}
