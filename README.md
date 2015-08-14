To create server and client binaries:
  make clean && make all

To run server:
  ./server [port] [hash size] [max threads] [thread idle time]

To run client
  ./client [host] [port]

To create 100000 random insert and find tests, 
1. run: make clean && make test
2. run server in another terminal
3. run: ./speed_test [host] [port]
