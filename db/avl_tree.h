#ifndef __AVLTREE_H__
#define __AVLTREE_H__

#include "avl_node.h"
#include "mutex.cc"
#include <limits>

template <class T>
class AVLTree {
  AVLMutex mutex;

  AVLNode<T>* root;
  void left_left(AVLNode<T>* target) {
    AVLNode<T>* temp = target->left;
    target->left = temp->right;
    if(temp->right != NULL)
      temp->right->parent = target;
    temp->right = target;

    target->height = temp->height - 1;
    target->replace(temp);
    target->parent = temp;

    if(root == target) root = temp;
  }

  void left_right(AVLNode<T>* target) {
    AVLNode<T>* child = target->left;
    AVLNode<T>* gchild = child->right;

    child->right = gchild->left;
    if(gchild->left != NULL)
      gchild->left->parent = child;
    
    gchild->left = child;
    child->replace(gchild);
    child->parent = gchild;

    child->height--;
    gchild->height++;

    left_left(target);
  }

  void right_right(AVLNode<T>* target) {
    AVLNode<T>* temp = target->right;
    // Fix temp's left child to target's right
    target->right = temp->left;
    if(temp->left != NULL)
      temp->left->parent = target;
    // Fix target to temp's left
    temp->left = target;

    target->height = temp->height - 1;
    // Fix temp's parents with target's parent
    target->replace(temp);
    // Fix target's parent
    target->parent = temp;
    
    if(root == target) root = temp;
  }

  void right_left(AVLNode<T>* target) {
    AVLNode<T>* child = target->right;
    AVLNode<T>* gchild = child->left;

    child->left = gchild->right;
    if(gchild->right != NULL)
      gchild->right->parent = child;

    gchild->right = child;
    child->replace(gchild);
    child->parent = gchild;

    child->height--;
    gchild->height++;

    right_right(target);
  }

  void balance(AVLNode<T>*& target) {
    AVLNode<T>* temp;
    if(target->balance() >= 2) {
      // Left subtree unbalanced
      if(target->left->balance() >= 0) {
        // larger subtree left: left-left case
        left_left(target);
      } else {
        // larger subtree right: left-right case
        left_right(target);
      }
    } else if(target->balance() <= -2) {
      // Right subtree unbalanced
      if(target->right->balance() > 0) {
        // larger subtree left: right-left case
        right_left(target);
      } else {
        // larget subtree right: right-right case
        right_right(target);
      }
    }
  }

  bool insert(T data, AVLNode<T>*& current, AVLNode<T>* parent) {
    bool success = false;

    if(current == NULL) {
      current = new AVLNode<T>(data, parent);
      return true;
    }

    if(*current > data) {
      success = insert(data, current->left, current);
    } else if(*current < data) {
      success = insert(data, current->right, current);
    } else if(*current == data) {
      return false;
    }

    current->calc_height();
    balance(current);

    return success;
  }

  AVLNode<T>* inorder_sucessor(AVLNode<T>* node) {
    AVLNode<T>* temp = node->right;
    while(temp->left != NULL) {
      temp = temp->left;
    }
    return temp;
  }

  AVLNode<T>* find(T data, AVLNode<T>* node) {
    if(node == NULL || *node == data) return node;
    if(*node > data) return find(data, node->left);
    else return find(data, node->right);
  }

  void delete_node(T data, AVLNode<T>*& node) {
    if(node == NULL || *node == data) {
      delete_node(node);
      return;
    }
    if(*node > data) delete_node(data, node->left);
    else delete_node(data, node->right);
    node->calc_height();
    balance(node);
  }

  void delete_node(AVLNode<T>* target) {
    if(target == NULL) return;

    if(target->is_leaf()) {
      target->seperate_from_parent();
      if(target == root) root = NULL;
      delete target;
    } else if(target->only_has_left()) {
      target->replace(target->left);
      if(target == root) root = target->left;
      delete target;
    } else if(target->only_has_right()) {
      target->replace(target->right);
      if(target == root) root = target->right;
      delete target;
    } else {
      AVLNode<T>* repl = inorder_sucessor(target);
      target->data = repl->data;
      repl->seperate_from_parent();
      delete repl;
    }
  }

   void display_tree(AVLNode<T>* node) {
    if(node == NULL) return;
    display_tree(node->left);
    //cout << node->data << ": h(" << node->height << ")" << ": b(" << node->balance() << "): p(" << (node->parent == NULL ? "No Parent" : node->parent->data) << ")" << ", ";
    cout << node->data << ", ";
    //cout << (node->left == NULL ? "NULL" : node->left->data) << "|" << node->data  << "|" << (node->right == NULL ? "NULL" : node->right->data) << " :p(" << (node->parent == NULL ? "No Parent" : node->parent->data) << "), ";
    //cout << (node->left == NULL ? -1 : node->left->data) << "|" << node->data  << "|" << (node->right == NULL ? -1 : node->right->data) << " :p(" << (node->parent == NULL ? -1 : node->parent->data) << "): b(" << node->balance() << "), ";

    display_tree(node->right);
  }

  string find(string data) {
    AVLNode<T>* result = find(data, root);
    if(result == NULL) return "";
    else return result->data;
  }

  int find(int data) {
    AVLNode<T>* result = find(data, root);
    if(result == NULL) return std::numeric_limits<int>::max();
    else return result->data;
  }

  float find(float data) {
    AVLNode<T>* result = find(data, root);
    if(result == NULL) return std::numeric_limits<float>::max();
    else return result->data;
  }

  public:
  AVLTree() {
    root = NULL;
  }

  bool insert(T data) {
    mutex.accquire_lock();
    bool ret_val = insert(data, root, NULL);
    mutex.unlock();
    mutex.signal_free();

    return ret_val;
  }

  void insert_many(T* datum, int size) {
    for (int i = 0; i < size; ++i) {
      insert(datum[i]);
    }
  }

  void display_tree() {
    if (root == NULL) {
      return;
    }
    display_tree(root);
  }

  void delete_node(T data) {
    mutex.accquire_lock();
    delete_node(data, root);
    mutex.unlock();
    mutex.signal_free();
  }

  T find_data(T data) {
    mutex.accquire_lock();
    T found = find(data);
    mutex.unlock();
    mutex.signal_free();

    return found;
  }

  bool confirm_data(T data) {
    AVLNode<T>* result = find(data, root);
    if(result == NULL) return false;
    else return true;
  }

  void delete_all() {
    if(root != NULL) {
      root->delete_tree();
      delete root;
      root = NULL;
    }
  }

  ~AVLTree() {
    if(root != NULL) {
      root->delete_tree();
      delete root;
      root = NULL;
    }
  }
};

/*
void double_imbalance() {
  AVLTree<int> tree;
  int array[] = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 55, 1};
  int size = 12;

  tree.insert_many(array, size);
  tree.display_tree();
  cout << endl;

  tree.delete_node(80);
  tree.display_tree();
  cout << endl;
}

void insert_tests() {
  AVLTree<int> tree;

  tree.insert(20);
  tree.insert(4);

  tree.display_tree(); cout << endl;
  tree.insert(8);
  tree.display_tree(); cout << endl;
  
  cout << endl << endl;
  AVLTree<int> tree1;

  tree1.insert(20);tree1.insert(4);
  tree1.insert(3);tree1.insert(9);tree1.insert(26);
  tree1.display_tree(); cout << endl;
  tree1.insert(8);
  tree1.display_tree(); cout << endl;
  
  cout << endl << endl;
  AVLTree<int> tree2;

  tree2.insert(20);
  tree2.insert(4); tree2.insert(26);
  tree2.insert(3); tree2.insert(9); tree2.insert(21); tree2.insert(30);
  tree2.insert(2); tree2.insert(7); tree2.insert(11);
  
  tree2.display_tree(); cout << endl;
  tree2.insert(8);
  tree2.display_tree(); cout << endl;
}

void delete_tests() {
  AVLTree<int> t1;
  AVLTree<int> t2;
  AVLTree<int> t3;

  t1.insert(2); t1.insert(1); t1.insert(4); t1.insert(3); t1.insert(5);
  t1.display_tree(); cout << endl;
  t1.delete_node(1);
  t1.display_tree(); cout << endl << endl;
  
  t2.insert(6); t2.insert(2); t2.insert(9); t2.insert(1); t2.insert(4); t2.insert(8); t2.insert(14); t2.insert(13); t2.insert(7); t2.insert(15); t2.insert(16); t2.insert(3); t2.insert(5);
  t2.display_tree(); cout << endl;
  t2.delete_node(1);
  t2.display_tree(); cout << endl << endl;
  

  t3.insert(5); t3.insert(2); t3.insert(8); t3.insert(1); t3.insert(3); t3.insert(7); t3.insert(13); t3.insert(4); t3.insert(6); t3.insert(9); t3.insert(14); t3.insert(15);
  t3.display_tree(); cout << endl;
  t3.delete_node(1);
  t3.display_tree(); cout << endl << endl;
}

*/
/*
int main(int argc, char *argv[])
{
  double_imbalance();
  
  return 0;
}
*/

#endif
