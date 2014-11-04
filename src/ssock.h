#ifndef SSOCK_H
#define SSOCK_H

#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// simplicity on probably one or two functions
typedef enum { false, true } bool;

// only necessary for af_inet, so segmented
typedef struct ssock_af_inet_settings_s {
  int port;
  struct sockaddr_in address;
  socklen_t addrlen;
} ssock_af_inet_settings_t;

// only necessary for af_unix, so segmented
typedef struct ssock_af_unix_settings_s {
  struct sockaddr_un address;
  char *path;
} ssock_af_unix_settings_t;

typedef struct ssock_settings_s {
  struct sockaddr address;
  socklen_t address_len;

  union {
    uint16_t port;
    char *path;
  };
} ssock_settings_t;

// this can be seriously cleaned up probably
typedef struct ssock_s {
  int socket, new_socket;
  int type;

  int backlog, bufsize;
  char * (* f)(struct ssock_s *);

  ssock_settings_t settings;

  char *buffer;
  char *response;
} ssock_t;

// yay for typedefs and function pointers
typedef char * (* ssock_func_t)(ssock_t *);

bool ssock_init (ssock_t *);
bool ssock_bind (ssock_t *);
bool ssock_listen (ssock_t *);
bool ssock_accept (ssock_t *);
ssize_t ssock_recv (ssock_t *);
ssize_t ssock_write (ssock_t *, char *msg);
void ssock_close (ssock_t *, int which);

#endif
