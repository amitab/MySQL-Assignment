all: avlnode.o avltree.o hashtable.o sharedds.o

avlnode.o:
	g++ -c avl_node.h

avltree.o: avlnode.o
	g++ -c avl_tree.h -lpthread

hashtable.o: avltree.o
	g++ -c hash_table.h -lpthread

sharedds.o: hashtable.o
	g++ -c shared_ds.cc -lpthread

clean:
	rm -f *.o
