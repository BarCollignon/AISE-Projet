CC=gcc

CFLAGS=-Wall
OFLAGS=-O3

all: server client

server: server.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

client: client.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

clean:
	rm -Rf *~ client server

