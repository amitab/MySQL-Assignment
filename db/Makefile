all: sharedds.o

sharedds.o:
	g++ -c shared_ds.cc -g

avltest: avl_node.h avl_tree.h main.cc
	g++ -g avl_node.h avl_tree.h avl_test.cc -o avltest.o -lpthread

clean:
	rm -f *.o
