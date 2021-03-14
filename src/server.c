#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#include<netdb.h>
#include<netinet/in.h>

#include<sys/socket.h>
#include<sys/types.h>

#include<pthread.h>

#include<ncurses.h>

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

  sock_data_t sock_data;
  sock_data.listen_sock=listen_sock;
  sock_data.nbclient=0;
  sock_data.client_data=NULL;

  pthread_t th;

  pthread_create(&th, NULL, accept_loop, (void *)&sock_data);

  WINDOW *left, *right;
    
  initscr();
  curs_set(FALSE);
  
  while(1){
    left = subwin(stdscr, LINES, COLS/2, 0, 0);
    right = subwin(stdscr, LINES, COLS/2, 0, COLS/2);
    werase(left);
    werase(right);

    for(int n=0; n < sock_data.nbclient; n++){
      if(n%2==0){
        wprintw(left, "\tMonitoring client %d\n", sock_data.client_data[n].client_socket-3);
        wprintw(left,"NB Cores : \t%d\n", sock_data.client_data[n].nbcore);
        wprintw(left,"CPU_GLOBAL : \t%lf %%\n", sock_data.client_data[n].percentages[0]);
        for(int i=1; i < sock_data.client_data[n].nbcore+1; i++)
          wprintw(left,"CPU%d : \t\t%lf %%\n", i, sock_data.client_data[n].percentages[i]);
        wprintw(left,"MemTotal: \t%d kB\n", sock_data.client_data[n].mem_total);
        wprintw(left,"MemAvailable: \t%d kB\n\n", sock_data.client_data[n].mem_available);
      }else{
        wprintw(right, "\tMonitoring client %d\n", sock_data.client_data[n].client_socket-3);
        wprintw(right,"NB Cores : \t%d\n", sock_data.client_data[n].nbcore);
        wprintw(right,"CPU_GLOBAL : \t%lf %%\n", sock_data.client_data[n].percentages[0]);
        for(int i=1; i < sock_data.client_data[n].nbcore+1; i++)
          wprintw(right,"CPU%d : \t\t%lf %%\n", i, sock_data.client_data[n].percentages[i]);
        wprintw(right,"MemTotal: \t%d kB\n", sock_data.client_data[n].mem_total);
        wprintw(right,"MemAvailable: \t%d kB\n\n", sock_data.client_data[n].mem_available);
      }
    }

    wrefresh(left);
    wrefresh(right);

    sleep(2);
  }

  endwin();

  pthread_join(th, NULL);
  
  close(listen_sock);
  return 0;
}
