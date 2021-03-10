#ifndef _libclient_h
#define _libclient_h

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

void json_print();

#endif
