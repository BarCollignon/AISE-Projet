#ifndef _libserver_h
#define _libserver_h

struct data_s{
  int client_socket;
  int nbcore;
  double *percentages;
  int mem_total;
  int mem_available;
};

typedef struct data_s data_t;

struct sock_data_s{
  int listen_sock;
  int nbclient;
  data_t *client_data;
};

typedef struct sock_data_s sock_data_t;


void *accept_loop( void *param );
void *client_loop( void *param );

#endif
