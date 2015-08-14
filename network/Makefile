CC = g++
CFLAGS = -O -lpthread -lreadline

all: worker.o client_thread.o client_queue_mutex.o custom_socket_base.o client.o server.o lazy_thread_pool.o custom_socket.o

worker.o: client_thread.o client_queue_mutex.o
	$(CC) -c worker.cc $(CFLAGS)

client_thread.o: custom_socket.o
	$(CC) -c client_thread.cc $(CFLAGS)

client_queue_mutex.o: 
	$(CC) -c client_queue_mutex.cc $(CFLAGS)

custom_socket_base.o:
	$(CC) -c custom_socket_base.cc $(CFLAGS)

client.o: custom_socket.o
	$(CC) -c client.cc $(CFLAGS)

server.o: custom_socket_base.o client_thread.o lazy_thread_pool.o
	$(CC) -c server.cc $(CFLAGS)

lazy_thread_pool.o: client_queue_mutex.o worker.o client_thread.o
	$(CC) -c lazy_thread_pool.cc $(CFLAGS)

custom_socket.o: custom_socket_base.o
	$(CC) -c custom_socket.cc $(CFLAGS)

clean:
	rm -f *.o

