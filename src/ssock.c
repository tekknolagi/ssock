#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssock.h"

bool ssock_init (ssock_t *sock, int bufsize) {
  sock->socket = socket(AF_INET, SOCK_STREAM, 0);
  bool success = sock->socket > 0;
  if (success) {
    sock->bufsize = bufsize;
    sock->buffer = calloc(bufsize, sizeof *sock->buffer);
    if (!sock->buffer) return false;
  }

  return success;
}

bool ssock_bind (ssock_t *sock, int port) {
  sock->address.sin_family = AF_INET;
  sock->address.sin_addr.s_addr = INADDR_ANY;
  sock->address.sin_port = htons(port);

  return bind(sock->socket, (struct sockaddr *) &sock->address,
	      sizeof sock->address) == 0;
}

bool ssock_listen (ssock_t *sock, int backlog) {
  return listen(sock->socket, backlog) == 0;
}

bool ssock_accept (ssock_t *sock) {
  sock->new_socket = accept(sock->socket, (struct sockaddr *) &sock->address,
			    &sock->addrlen);
  return sock->new_socket > 0;
}

void ssock_recv (ssock_t *sock) {
  recv(sock->new_socket, sock->buffer, sock->bufsize, 0);
}

void ssock_write (ssock_t *sock, char *msg) {
  write(sock->new_socket, msg, strlen(msg));
}

void ssock_close (ssock_t *sock, int which) {
  if (which == 1)
    close(sock->new_socket);
  else if (which == 2) {
    close(sock->socket);
    free(sock->buffer);
  }
}
