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

  sock_data_t *sock_data = (sock_data_t *)param;

  struct sockaddr client_info;
  socklen_t addr_len;

  while(1){
    int cs = accept( sock_data->listen_sock, &client_info, &addr_len );

    if(cs < 0){
      perror("accept");
      continue;
    }
    sock_data->nbclient++;

    sock_data->client_data=(data_t *)realloc(sock_data->client_data,sock_data->nbclient*sizeof(data_t));

    sock_data->client_data[sock_data->nbclient-1].client_socket = cs;

    pthread_t th;

    pthread_create(&th, NULL, client_loop, (void *)&sock_data->client_data[sock_data->nbclient-1]);
  }

}

void *client_loop( void *param ){

  data_t *client_data=(data_t *)param;

  int ret;

  ret = read(client_data->client_socket, (void *)&client_data->nbcore, sizeof(int));

  client_data->percentages=(double *)malloc((client_data->nbcore+1)*sizeof(double));

  while(1){
    for(int i=0; i < client_data->nbcore+1; i++){
      ret=read(client_data->client_socket, (void *)&client_data->percentages[i], sizeof(double));
      if(ret<=0)
        perror("read");
    }

    ret=read(client_data->client_socket, (void *)&client_data->mem_total, sizeof(int));
    if(ret<=0)
      perror("read");
    
    ret=read(client_data->client_socket, (void *)&client_data->mem_available, sizeof(int));
    if(ret<=0)
      perror("read");
    
  }
}
