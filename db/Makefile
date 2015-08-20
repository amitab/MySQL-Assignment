all: sharedds.o

sharedds.o:
	g++ -c shared_ds.cc -O2

avltest: avl_node.h avl_tree.h avl_test.cc
	g++ -g avl_node.h avl_tree.h avl_test.cc -o avltest.o -lpthread

dstest: avl_node.h avl_tree.h hash_table.h shared_ds.cc
	g++ -g -std=c++11  avl_node.h avl_tree.h hash_table.h shared_ds.cc tests.cc -o sharedtest.o -lpthread

balancetest: avl_node.h avl_tree.h
	g++ -isystem ../gtest/include -pthread test_case.cc libgtest.a -o balance_test.o

clean:
	rm -f *.o
