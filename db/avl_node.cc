#include <iostream>
using namespace std;

template <class T>
class AVLNode {
public:
  int height;
  AVLNode* left;
  AVLNode* right;
  AVLNode* parent;
  T data;

  AVLNode(T data, AVLNode* parent) {
    this->data = data;
    this->parent = parent;
    left = right = NULL;
    height = 0;
  }

  void replace(AVLNode*& target) {
    target->parent = parent;
    if(parent == NULL) return;
    if(*parent > data) {
      parent->left = target;
    }
    else {
      parent->right = target;
    }

  }

  bool is_leaf() {
    return (left == NULL && right == NULL);
  }

  bool only_has_left() {
    return (left != NULL && right == NULL);
  }

  bool only_has_right() {
    return (left == NULL && right != NULL);
  }

  bool has_both() {
    return (left != NULL && right != NULL);
  }

  void seperate_from_parent() {
    if(parent == NULL) return;
    if(*parent > data) parent->left = NULL;
    else parent->right = NULL;
  }

  int balance() {
    int right_h = (right == NULL ? 0 : right->height + 1);
    int left_h (left == NULL ? 0 : left->height + 1);
    
    return left_h - right_h;
  }

  void calc_height() {
    if(is_leaf()) height = 0;
    else {
      height = max((left == NULL ? 0 : left->height), (right == NULL ? 0 : right->height)) + 1;
    }
  }

  bool operator>(const AVLNode& node) {
    return data > node.data;
  }

  bool operator<(const AVLNode& node) {
    return data < node.data;
  }

  bool operator==(const AVLNode& node) {
    return data == node.data;
  }

  bool operator>(T value) {
    return data > value;
  }

  bool operator<(T value) {
    return data < value;
  }

  bool operator==(T value) {
    return data == value;
  }

  void delete_tree() {
    if(left != NULL) {
      left->delete_tree();
      delete left;
      left = NULL;
    }

    if(right != NULL) {
      right->delete_tree();
      delete right;
      right = NULL;
    }
  }

};
