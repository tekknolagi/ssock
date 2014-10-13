#ifndef SSOCK_H
#define SSOCK_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum { false, true } bool;

typedef struct ssock_s {
  int socket, new_socket;
  struct sockaddr_in address;
  socklen_t addrlen;

  int bufsize;
  char *buffer;
} ssock_t;

bool ssock_init (ssock_t *, int bufsize);
bool ssock_bind (ssock_t *, int port);
bool ssock_listen (ssock_t *);
bool ssock_accept (ssock_t *);
void ssock_recv (ssock_t *);
void ssock_write (ssock_t *, char *msg);
void ssock_close (ssock_t *, int which);

#endif
