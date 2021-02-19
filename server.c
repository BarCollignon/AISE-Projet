#include<stdio.h>
#include<unistd.h>
#include<stdlib.h> 
#include<string.h> 

#include<netdb.h> 
#include<netinet/in.h> 

#include<sys/socket.h> 
#include<sys/types.h> 

#include<pthread.h>


void *client_loop( void *param ){

  int *client_socket=(int *)param;

  int ret;
  double *percentage=(double *)malloc(sizeof(double));

  while(1){
    ret=read(*client_socket, (void *)percentage, sizeof(double));
    if(ret<=0)
      perror("read");

    printf("[%d]CPU_GLOBAL : %lf%%\n", *client_socket-3, *percentage);
  }
}

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

  struct sockaddr client_info;
  socklen_t addr_len;

  while(1){
    int cs = accept( listen_sock, &client_info, &addr_len );
    fprintf( stderr, "Client accept\n");

    if(cs < 0){
      perror("accept");
      return 1;
    }

    int *client_socket = (int *)malloc(sizeof(int));

    *client_socket = cs;

    pthread_t th;

    pthread_create(&th, NULL, client_loop, (void *)client_socket);
  }


  close(listen_sock);
  return 0;
}
