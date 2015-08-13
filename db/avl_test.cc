#include "avl_tree.h"

int main(int argc, char *argv[])
{
  AVLTree<int> tree;
  for (int i = 0; i < 10000; ++i) {
    tree.insert(i);
  }
  tree.display_tree();
  return 0;
}

