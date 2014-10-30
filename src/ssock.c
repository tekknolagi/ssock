#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ssock.h"

bool ssock_init (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->settings.bufsize > 0);

  sock->socket = socket(AF_INET, SOCK_STREAM, 0);
  bool success = sock->socket > 0;
  if (success) {
    sock->buffer = calloc(sock->settings.bufsize, sizeof *sock->buffer);
    if (!sock->buffer) return false;
  }

  return success;
}

bool ssock_bind (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->settings.port > 0);

  sock->address.sin_family = AF_INET;
  sock->address.sin_addr.s_addr = INADDR_ANY;
  sock->address.sin_port = htons(sock->settings.port);

  return bind(sock->socket, (struct sockaddr *) &sock->address,
	      sizeof sock->address) == 0;
}

bool ssock_listen (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->settings.backlog > 0);

  return listen(sock->socket, sock->settings.backlog) == 0;
}

bool ssock_accept (ssock_t *sock) {
  assert(sock != NULL);

  sock->new_socket = accept(sock->socket, (struct sockaddr *) &sock->address,
			    &sock->addrlen);
  return sock->new_socket > 0;
}

void ssock_recv (ssock_t *sock) {
  assert(sock != NULL);

  recv(sock->new_socket, sock->buffer, sock->settings.bufsize, 0);
}

void ssock_write (ssock_t *sock, char *msg) {
  assert(sock != NULL);
  assert(msg != NULL);

  write(sock->new_socket, msg, strlen(msg));
}

void ssock_close (ssock_t *sock, int which) {
  assert(sock != NULL);

  if (which == 1)
    close(sock->new_socket);
  else if (which == 2) {
    close(sock->socket);
    free(sock->buffer);
  }
}
