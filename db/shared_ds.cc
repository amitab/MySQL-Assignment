#include "hash_table.cc"
#include <sstream>

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

  bool insert_data(string data) {
    str_count++;
    return string_ds.insert_data(data);
  }

  bool insert_data(int data) {
    int_count++;
    return int_ds.insert_data(data);
  }

  bool insert_data(float data) {
    float_count++;
    return float_ds.insert_data(data);
  }

  void delete_data(string data) {
    str_count--;
    string_ds.delete_data(data);
  }

  void delete_data(int data) {
    int_count--;
    int_ds.delete_data(data);
  }

  void delete_data(float data) {
    float_count--;
    float_ds.delete_data(data);
  }

  string find_data(string data) {
    return string_ds.find_data(data);
  }

  int find_data(int data) {
    return int_ds.find_data(data);
  }

  float find_data(float data) {
    return float_ds.find_data(data);
  }

  std::string status() {
    std::stringstream status_str;
    status_str << "Float count: " << float_count << "\n";
    status_str << "Int count: " << int_count << "\n";
    status_str << "String count: " << str_count << "\n";

    return status_str.str();
  }
};
