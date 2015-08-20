#include "avl_tree.h"
#include "gtest/gtest.h"

TEST(BalanceTest, DeleteDoubleImbalance) {
  AVLTree<int> tree;
  int array[] = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 55, 1};

  tree.insert_many(array, 12);
  tree.delete_node(80);
  EXPECT_TRUE(tree.check_balance());
}

TEST(BalanceTest, InsertImbalance) {
  AVLTree<int> tree1;
  int data1[] = {20, 4, 3, 9, 26};
  int data2[] = {20, 4, 26, 3, 9, 21, 30, 2, 7, 11};

  tree1.insert_many(data1, 5);
  tree1.insert(8);
  EXPECT_TRUE(tree1.check_balance());

  AVLTree<int> tree2;
  tree2.insert_many(data2, 10);
  tree2.insert(8);
  EXPECT_TRUE(tree2.check_balance());
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_death_test_style = "fast";
  return RUN_ALL_TESTS();
}
