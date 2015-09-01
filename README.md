To create server and client binaries:
  make clean && make all

To run server:
  ./server [port] [hash size] [max threads] [thread idle time]

To run client
  ./client [host] [port]

To run test for 5 clients working for 30 minutes:
1.  run: make clean && make test
2.  run server in another terminal
3.  run ./test_case [host] [port]

To create n random insert and find tests, 
1.  run: make clean && make speedtest
2.  run server in another terminal
3.  run: ./speed_test [host] [port] [number of inserts and finds] 
    example: ./speed_test 127.0.0.1 5001 10000
    would run a speed test for 10000 inserts and 10000 finds

Client Application has the following commands available:
1. INSERT|DELETE|FIND [int]|[float]|[string]
2. SHOW ALL|STATUS|CLIENT DETAILS

example:
INSERT 21
FIND "sdsd"
DELETE 53.33
