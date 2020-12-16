# Makefile for TCP example
all: cliente-tcp server-tcp intermedio-tcp cliente-udp server-udp clean
cliente-tcp: cli-tcp.c
	gcc -g cli-tcp.c -o cliente-tcp

server-tcp: serv-tcp.c
	gcc -g serv-tcp.c -o server-tcp

intermedio-tcp: int-tcp.c
	gcc -g int-tcp.c -o intermedio-tcp

cliente-udp: udp-client.o Socket.o
	g++ -o cliente-udp udp-client.o Socket.o -lssl -lcrypto

server-udp: udp-server.o Socket.o
	g++ -o server-udp udp-server.o Socket.o -lssl -lcrypto
	
udp-client.o: udp-client.cc Socket.h
	g++ -c udp-client.cc Socket.h

udp-server.o: udp-server.cc Socket.h
	g++ -c udp-server.cc Socket.h

Socket.o: Socket.cc Socket.h
	g++ -c Socket.cc Socket.h -lssl -lcrypto
	
clean:
	rm -f *.o *.gch
