#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 

#include <netdb.h> 
#include <netinet/in.h>

#include <sys/socket.h> 


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

  /*struct addrinfo *res = NULL;
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
  */

/// STAT  

  int ret, prevIdle, currIdle, prevNonIdle, currNonIdle, prevTotal, currTotal, total, idled;
  double percentage;

  struct stats prev;
  struct stats current;

  while(1){
    memset(&prev, 0, sizeof(prev));
    memset(&current, 0, sizeof(current));

    FILE *fd=fopen("/proc/stat","r");
    if(!fd)
      return 1;
    ret = fscanf(fd,"%s %d %d %d %d %d %d %d %d %d %d\n", prev.cpu, &prev.user, &prev.nice, &prev.system, &prev.idle, &prev.iowait, &prev.irq, &prev.softirq, &prev.steal, &prev.guest, &prev.guest_nice);
    if(ret!=11)
      perror("fscanf");
    fclose(fd);

    usleep(200000);//0.2 Seconds // 200 Milliseconds

    fd=fopen("/proc/stat","r");
    if(!fd)
      return 1;
    ret = fscanf(fd,"%s %d %d %d %d %d %d %d %d %d %d\n", current.cpu, &current.user, &current.nice, &current.system, &current.idle, &current.iowait, &current.irq, &current.softirq, &current.steal, &current.guest, &current.guest_nice);
    if(ret!=11)
      perror("fscanf");
    fclose(fd);

    //printf("%s %d %d %d %d %d %d %d %d %d %d\n", prev.cpu, prev.user, prev.nice, prev.system, prev.idle, prev.iowait, prev.irq, prev.softirq, prev.steal, prev.guest, prev.guest_nice);
    //printf("%s %d %d %d %d %d %d %d %d %d %d\n", current.cpu, current.user, current.nice, current.system, current.idle, current.iowait, current.irq, current.softirq, current.steal, current.guest, current.guest_nice);

  
    prevIdle = prev.idle + prev.iowait;
    currIdle = current.idle + current.iowait;

    prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    currNonIdle = current.user + current.nice + current.system + current.irq + current.softirq + current.steal;

    prevTotal = prevIdle+prevNonIdle;
    currTotal = currIdle+currNonIdle;

    total = currTotal-prevTotal;
    idled = currIdle-prevIdle;

    percentage = (double)((total-idled))/(double)(total*100);
    percentage *= 10000;

    printf("CPU_GLOBAL_PERCENTAGE : %lf%%\n\n", percentage);
    sleep(2);
  }
/// SEND STAT IN SOCKET

  //close(sock);
  return 0;
} 

