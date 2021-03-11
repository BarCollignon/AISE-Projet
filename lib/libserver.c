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

void *accept_loop( void *param ){

  int *listen_sock = (int *)param;

  struct sockaddr client_info;
  socklen_t addr_len;

  while(1){
    int cs = accept( *listen_sock, &client_info, &addr_len );

    if(cs < 0){
      perror("accept");
      continue;
    }

    int *client_socket = (int *)malloc(sizeof(int));

    *client_socket = cs;

    pthread_t th;

    pthread_create(&th, NULL, client_loop, (void *)client_socket);
  }

}

void *client_loop( void *param ){

  int *client_socket=(int *)param;

  int ret;

  int nbcore;
  ret = read(*client_socket, (void *)&nbcore, sizeof(int));

  double *percentage=(double *)malloc((nbcore+1)*sizeof(double));

  while(1){
    for(int i=0; i < nbcore+1; i++)
      ret=read(*client_socket, (void *)&percentage[i], sizeof(double));

    if(ret<=0)
      perror("read");

    printf("[%d]NB Cores : %d\n", *client_socket-3, nbcore);
    printf("CPU_GLOBAL : %lf%%\n", percentage[0]);
    for(int i=1; i < nbcore+1; i++)
      printf("CPU%d : %lf%%\n", i, percentage[i]);
  }
}
