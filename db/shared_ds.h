#ifndef __SHAREDDS_H__
#define __SHAREDDS_H__

#include "hash_table.h"

class SharedDS {
  public:
  HashTable<string> string_ds;
  HashTable<float> float_ds;
  HashTable<int> int_ds;

  int float_count;
  int int_count;
  int str_count;

  SharedDS(int size): string_ds(size), float_ds(size), int_ds(size) {
    float_count = 0;
    int_count = 0;
    str_count = 0;
  }
  
  void delete_all();
  
  bool insert_data(string data);
  bool insert_data(int data);
  bool insert_data(float data);
  
  void delete_data(string data);
  void delete_data(int data);
  void delete_data(float data);
  
  string find_data(string data);
  int find_data(int data);
  float find_data(float data);
  
  bool confirm_data(int data);
  bool confirm_data(float data);
  bool confirm_data(string data);
  
  std::string status();
  void show();
};

#endif
