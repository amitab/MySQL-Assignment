#include <iostream>
#include <cmath>
#include "avl_tree.cc"
using namespace std;

template <class T>
class HashTable {
  public:
  AVLTree<T>** buckets;
  int size;

  HashTable(int size) {
    buckets = new AVLTree<T>*[size];
    for (int i = 0; i < size; ++i) {
      buckets[i] = new AVLTree<T>();
    }
    this->size = size;
  }

  unsigned long awesome_hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % size;
  }

  unsigned long awesome_hash(string data) {
    return awesome_hash(data.c_str());
  }

  unsigned int awesome_hash(int data) {
    unsigned int x = (unsigned int) data;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    return x % size;
  }

  unsigned int awesome_hash(float data) {
    unsigned int x = ceil(data);
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    return x % size;
  }

  bool insert_data(T data) {
    unsigned int key = awesome_hash(data);
    return buckets[key]->insert(data);
  }

  void delete_data(T data) {
    unsigned int key = awesome_hash(data);
    buckets[key]->delete_node(data);
  }

  T find_data(T data) {
    unsigned int key = awesome_hash(data);
    return buckets[key]->find_data(data);
  }

  void display() {
    for (int i = 0; i < size; ++i) {
      buckets[i]->display_tree();
    }
  }

  ~HashTable() {
    for (int i = 0; i < size; ++i) {
      delete buckets[i];
    }
    delete[] buckets;
    buckets = NULL;
  }

};

/*
int main() {
  HashTable<float> hash(100);

  for (int i = 0; i < 108; ++i) {
    hash.insert_data(i + 0.2);
  }
  hash.display();
  cout << "Found at: " << hash.find_data(8.2) << endl;
  hash.delete_data(8.2);
  cout << "Done deleting" << endl;
  cout << "Found at: " << hash.find_data(8.2) << endl;
  hash.display();
  return 0;
}
*/
