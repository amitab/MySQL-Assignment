#include "avl_tree.h"

int main(int argc, char *argv[])
{
  AVLTree<int> tree;
  for (int i = 0; i < 100; ++i) {
    tree.insert(i);
  }
  cout << tree.display_tree() << endl;
  return 0;
}

