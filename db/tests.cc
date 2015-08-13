#include "network/client.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
  high_resolution_clock::time_point t1;
  high_resolution_clock::time_point t2;
  long duration;
  
  SharedDS ds(111);

  t1 = high_resolution_clock::now();
  for (int i = 0; i < 10000; ++i) {
    ds.insert_data(i);
  }
  t2 = high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  cout << "10000 insert time: " << duration << endl;

  t1 = high_resolution_clock::now();
  for (int i = 0; i < 10000; ++i) {
    ds.find_data(i);
  }
  t2 = high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  cout << "10000 search time: " << duration << endl;

  return 0;
}
