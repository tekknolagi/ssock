#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ssock.h"

bool ssock_init (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->settings.af_inet.bufsize > 0);

  sock->socket = socket(sock->type, SOCK_STREAM, 0);
  bool success = sock->socket > 0;
  if (success) {
    sock->buffer = calloc(sock->settings.af_inet.bufsize, sizeof *sock->buffer);
    if (!sock->buffer) return false;
  }

  return success;
}

bool ssock_bind (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->type) {
  case AF_INET: {
    assert(sock->settings.af_inet.port > 0);

    sock->settings.af_inet.address.sin_family = AF_INET;
    sock->settings.af_inet.address.sin_addr.s_addr = INADDR_ANY;
    sock->settings.af_inet.address.sin_port = htons(sock->settings.af_inet.port);
    return bind(sock->socket, (struct sockaddr *) &sock->settings.af_inet.address,
		sizeof sock->settings.af_inet.address) == 0;
    break;
  }
  default: {
    printf("BAD SOCK TYPE.\n");
    exit(1);
  }
  }
}

bool ssock_listen (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->settings.af_inet.backlog > 0);

  return listen(sock->socket, sock->settings.af_inet.backlog) == 0;
}

bool ssock_accept (ssock_t *sock) {
  assert(sock != NULL);

  sock->new_socket = accept(sock->socket, (struct sockaddr *) &sock->settings.af_inet.address,
			    &sock->settings.af_inet.addrlen);
  return sock->new_socket > 0;
}

void ssock_recv (ssock_t *sock) {
  assert(sock != NULL);

  recv(sock->new_socket, sock->buffer, sock->settings.af_inet.bufsize, 0);
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
