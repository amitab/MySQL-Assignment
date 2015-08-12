NETWORK_SERVER = network/client_queue_mutex.o network/client_thread.o network/custom_socket_base.o network/custom_socket.o network/lazy_thread_pool.o network/server.o network/worker.o
NETWORK_CLIENT = network/client.o network/custom_socket_base.o network/custom_socket.o
DB = db/avl_node.o db/avl_tree.o db/hash_table.o db/shared_ds.o
PARSER = parser/client_parser.o parser/Expression.o parser/Lexer.o parser/Parser.o
CC = g++
CFLAGS = -g -lpthread -lreadline

makeserver: network db parser
	$(CC) $(NETWORK_SERVER) $(PARSER) $(DB) server_process.cc -o server $(CFLAGS)

makeclient:network
	$(CC) $(NETWORK_CLIENT) client_process.cc -o client $(CFLAGS)

parser:
	cd parser && $(MAKE) all

db:
	cd db && $(MAKE) all

network:
	cd network && $(MAKE) all
