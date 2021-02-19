#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 

#include <netdb.h> 
#include <netinet/in.h>

#include <sys/socket.h> 

#define NBCORE 3

struct stats{
  char cpu[4];
  int user;
  int nice;
  int system;
  int idle;
  int iowait;
  int irq;
  int softirq;
  int steal;
  int guest;
  int guest_nice;
}; 




//
int main(int argc, char **argv){
 
  if(argc < 3)
    return 1;

/// CREATE AND CONNECT SOCKET

  struct addrinfo *res = NULL;
  struct addrinfo hints;
  struct addrinfo *tmp;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int ret = getaddrinfo(argv[1], argv[2], &hints, &res);
  if(ret < 0){
    herror("getaddrinfo");
    return 1;
  }

  int sock = -1;
  int connected = 0;

  for(tmp = res; tmp != NULL; tmp = tmp->ai_next){

    sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
    if(sock < 0){
      perror("socket");
      continue;
    }

    ret = connect(sock, tmp->ai_addr, tmp->ai_addrlen);
    if(ret < 0){
      close(sock);
      perror("connect");
      continue;
    }

    connected = 1;
    break;
  }

  printf("Socket successfully created..\n");

  if(!connected){
    fprintf(stderr, "Failed to connect to %s:%s\n", argv[1], argv[2]);
    return 1;
  }

  printf("connected to the server..\n"); 
  

/// STAT  

  int prevIdle, currIdle, prevNonIdle, currNonIdle, prevTotal, currTotal, total, idled;
  double percentage;

  struct stats prev[9];
  struct stats current[9];

  while(1){
    memset(&prev, 0, sizeof(prev));
    memset(&current, 0, sizeof(current));

    FILE *fd=fopen("/proc/stat","r");
    if(!fd)
      return 1;
    for(int i=0; i < NBCORE+1; i++){
      ret = fscanf(fd,"%s %d %d %d %d %d %d %d %d %d %d\n", prev[i].cpu, &prev[i].user, &prev[i].nice, &prev[i].system, &prev[i].idle, &prev[i].iowait, &prev[i].irq, &prev[i].softirq, &prev[i].steal, &prev[i].guest, &prev[i].guest_nice);
      if(ret!=11)
        perror("fscanf");
    }

    fclose(fd);

    usleep(200000);//0.2 Seconds // 200 Milliseconds

    fd=fopen("/proc/stat","r");
    if(!fd)
      return 1;
    for(int i=0; i < NBCORE+1; i++){
      ret = fscanf(fd,"%s %d %d %d %d %d %d %d %d %d %d\n", current[i].cpu, &current[i].user, &current[i].nice, &current[i].system, &current[i].idle, &current[i].iowait, &current[i].irq, &current[i].softirq, &current[i].steal, &current[i].guest, &current[i].guest_nice);
      if(ret!=11)
        perror("fscanf");
    }

    fclose(fd);

    //printf("%s %d %d %d %d %d %d %d %d %d %d\n", prev[i].cpu, prev[i].user, prev[i].nice, prev[i].system, prev[i].idle, prev[i].iowait, prev[i].irq, prev[i].softirq, prev[i].steal, prev[i].guest, prev[i].guest_nice);
    //printf("%s %d %d %d %d %d %d %d %d %d %d\n", current[i].cpu, current[i].user, current[i].nice, current[i].system, current[i].idle, current[i].iowait, current[i].irq, current[i].softirq, current[i].steal, current[i].guest, current[i].guest_nice);

    for(int i=1; i < NBCORE+1; i++){
      prevIdle = prev[i].idle + prev[i].iowait;
      currIdle = current[i].idle + current[i].iowait;

      prevNonIdle = prev[i].user + prev[i].nice + prev[i].system + prev[i].irq + prev[i].softirq + prev[i].steal;
      currNonIdle = current[i].user + current[i].nice + current[i].system + current[i].irq + current[i].softirq + current[i].steal;

      prevTotal = prevIdle+prevNonIdle;
      currTotal = currIdle+currNonIdle;

      total = currTotal-prevTotal;
      idled = currIdle-prevIdle;

      percentage = (double)((total-idled))/(double)(total*100);
      percentage *= 10000;

    
      printf("CPU%d : %lf%%\n", i, percentage);

    }

    prevIdle = prev[0].idle + prev[0].iowait;
    currIdle = current[0].idle + current[0].iowait;

    prevNonIdle = prev[0].user + prev[0].nice + prev[0].system + prev[0].irq + prev[0].softirq + prev[0].steal;
    currNonIdle = current[0].user + current[0].nice + current[0].system + current[0].irq + current[0].softirq + current[0].steal;

    prevTotal = prevIdle+prevNonIdle;
    currTotal = currIdle+currNonIdle;

    total = currTotal-prevTotal;
    idled = currIdle-prevIdle;

    percentage = (double)((total-idled))/(double)(total*100);
    percentage *= 10000;
   
    printf("CPU_GLOBAL : %lf%%\n", percentage);



    sleep(2);
    ret=system("clear");
  }


/// SEND STAT IN SOCKET



  close(sock);
  return 0;
} 

