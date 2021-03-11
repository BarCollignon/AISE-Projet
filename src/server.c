#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#include<netdb.h>
#include<netinet/in.h>

#include<sys/socket.h>
#include<sys/types.h>

#include<pthread.h>
#include "libserver.h"
//
int main(int argc, char **argv){

  if(argc < 2)
    return 1;

  struct addrinfo *res = NULL;
  struct addrinfo hints;
  struct addrinfo *tmp;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int ret = getaddrinfo(NULL, argv[1], &hints, &res);
  if(ret < 0){
    herror("getaddrinfo");
    return 1;
  }

  int listen_sock = -1;
  int binded = 0;

  for(tmp = res; tmp != NULL; tmp = tmp->ai_next){

    listen_sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
    if(listen_sock < 0){
      perror( "socket" );
      continue;
    }

    ret = bind(listen_sock, tmp->ai_addr, tmp->ai_addrlen);
    if(ret < 0){
      close(listen_sock);
      perror("bind");
      continue;
    }

    binded = 1;
    break;
  }

  printf("Socket successfully created..\n");

  if(!binded){
    fprintf(stderr, "Failed to bind on 0.0.0.0:%s\n", argv[1]);
    return 1;
  }

  printf("Socket successfully binded..\n");

  ret = listen(listen_sock, 2);
  if(ret < 0){
    perror("listen");
    return 1;
  }else{
    printf("Server listening..\n");
  }

  pthread_t th;

  pthread_create(&th, NULL, accept_loop, (void *)&listen_sock);

  pthread_join(th, NULL);
  

  close(listen_sock);
  return 0;
}
