#include "shared_ds.h"
#include <sstream>

void SharedDS::delete_all() {
  float_count = 0;
  int_count = 0;
  str_count = 0;

  string_ds.delete_all();
  float_ds.delete_all();
  int_ds.delete_all();
}

bool SharedDS::insert_data(string data) {
  str_count++;
  return string_ds.insert_data(data);
}

bool SharedDS::insert_data(int data) {
  int_count++;
  return int_ds.insert_data(data);
}

bool SharedDS::insert_data(float data) {
  float_count++;
  return float_ds.insert_data(data);
}

void SharedDS::delete_data(string data) {
  str_count--;
  string_ds.delete_data(data);
}

void SharedDS::delete_data(int data) {
  int_count--;
  int_ds.delete_data(data);
}

void SharedDS::delete_data(float data) {
  float_count--;
  float_ds.delete_data(data);
}

string SharedDS::find_data(string data) {
  return string_ds.find_data(data);
}

int SharedDS::find_data(int data) {
  return int_ds.find_data(data);
}

float SharedDS::find_data(float data) {
  return float_ds.find_data(data);
}

bool SharedDS::confirm_data(int data) {
  return int_ds.confirm_data(data);
}

bool SharedDS::confirm_data(float data) {
   return float_ds.confirm_data(data);
}

bool SharedDS::confirm_data(string data) {
  return string_ds.confirm_data(data);
}

std::string SharedDS::status() {
  std::stringstream status_str;
  status_str << "Float count: " << float_count << "\n";
  status_str << "Int count: " << int_count << "\n";
  status_str << "String count: " << str_count;

  return status_str.str();
}

void SharedDS::show() {
  string_ds.display();
  float_ds.display();
  int_ds.display();
  cout << endl;
}

/*
int main(int argc, char *argv[])
{
  SharedDS ds(100);
  for (int i = 0; i < 100; ++i) {
    ds.insert_data(i);
  }
  ds.show();
  ds.delete_data(80);
  ds.show();
  ds.delete_all();
  ds.show();
  return 0;
}
*/
