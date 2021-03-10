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
