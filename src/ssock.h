#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum { false, true } bool;

typedef struct sock_s {
  int socket, new_socket;
  struct sockaddr_in address;
  socklen_t addrlen;

  int bufsize;
  char *buffer;
} sock_t;

bool sock_init (sock_t *, int bufsize);
bool sock_bind (sock_t *, int port);
bool sock_listen (sock_t *);
bool sock_accept (sock_t *);

void sock_recv (sock_t *);
void sock_write (sock_t *, char *msg);
void sock_close (sock_t *, int which);
