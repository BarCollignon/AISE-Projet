CC=gcc
CFLAGS=-Wall
OFLAGS=-O3
INCLUDE = ./headers/

all: ./bin/server ./bin/client
server : ./bin/server
client : ./bin/client
clean : cleanmaster

#Binaries
./bin/server: ./src/server.c ./lib/libServer.a
	$(CC) $< -I$(INCLUDE) -L./lib/ -lServer -o $@ $(CFLAGS) -lpthread -lncurses

./bin/client: ./src/client.c ./lib/libClient.a
	$(CC) $< -I$(INCLUDE) -L./lib/ -lClient -o $@ $(CFLAGS)

#LIBRAIRIES
./lib/libServer.a : ./lib/libserver.c ./headers/libserver.h
	$(CC) -o ./lib/libserver.o -c $< -I$(INCLUDE) $(CFLAGS)
	ar rcs $@ ./lib/libserver.o

./lib/libClient.a : ./lib/libclient.c ./headers/libclient.h
	$(CC) -o ./lib/libclient.o -c $< -I$(INCLUDE) $(CFLAGS)
	ar rcs $@ ./lib/libclient.o

#CLEAN
cleanmaster :
	make cleanbin
	make cleanlib

cleanlib :
	rm -f ./lib/*.a
	rm -f ./lib/*.so
	rm -f ./lib/*.o

cleanbin :
	rm -f ./bin/*
