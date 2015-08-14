NETWORK_SERVER = network/client_queue_mutex.o network/client_thread.o network/custom_socket_base.o network/custom_socket.o network/lazy_thread_pool.o network/server.o network/worker.o
NETWORK_CLIENT = network/client.o network/custom_socket_base.o network/custom_socket.o
DB = db/avl_node.h db/avl_tree.h db/hash_table.h db/shared_ds.o
PARSER = parser/client_parser.o parser/Expression.o parser/Lexer.cc parser/Parser.cc
CC = g++
CFLAGS = -O2 -lpthread -lreadline

all: server client

speedtest: server client
	$(CC) -std=c++11 $(NETWORK_CLIENT) tests.cc -o speed_test $(CFLAGS)

test: server client
	$(CC) -std=c++11 $(NETWORK_CLIENT) test_case.cc -o test_case $(CFLAGS)

parser_files:
	(cd parser; make all)

db_files:
	cd db && make all

network_files:
	cd network && make all

server: parser_files network_files db_files
	$(CC) $(NETWORK_SERVER) $(PARSER) $(DB) server_process.cc -o server $(CFLAGS)

client: network_files
	$(CC) $(NETWORK_CLIENT) client_process.cc -o client $(CFLAGS)

clean_network:
	cd network && $(MAKE) clean

clean_db:
	cd db && $(MAKE) clean

clean_parser:
	cd parser && $(MAKE) clean

clean_bin:
	rm -f server client speed_test

clean: clean_network clean_db clean_parser clean_bin
