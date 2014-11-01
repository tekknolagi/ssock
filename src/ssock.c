#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ssock.h"

// any
bool ssock_init (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->bufsize > 0);

  sock->socket = socket(sock->type, SOCK_STREAM, 0);
  bool success = sock->socket > 0;

  if (success) {
    sock->buffer = calloc(sock->bufsize, sizeof *sock->buffer);
    if (!sock->buffer) return false;
  }

  return success;
}

// specific
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
  case AF_UNIX: {
    assert(sock->settings.af_unix.path != NULL);

    sock->settings.af_unix.address.sun_family = AF_UNIX;
    strncpy(sock->settings.af_unix.address.sun_path,
	    sock->settings.af_unix.path,
	    strlen(sock->settings.af_unix.path));
    printf("path: %s\n", sock->settings.af_unix.path);
    unlink(sock->settings.af_unix.address.sun_path);

    return bind(sock->socket, (struct sockaddr *) &sock->settings.af_unix.address,
		sizeof sock->settings.af_unix.address) == 0;

    break;
  }
  default: {
    printf("BAD SOCK TYPE.\n");
    return false;
  }
  }
}

// any
bool ssock_listen (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->backlog > 0);

  return listen(sock->socket, sock->backlog) == 0;
}

// specific
bool ssock_accept (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->type) {
  case AF_INET: {
    sock->new_socket = accept(sock->socket, (struct sockaddr *) &sock->settings.af_inet.address,
			      &sock->settings.af_inet.addrlen);
    break;
  }
  case AF_UNIX: {
    sock->new_socket = accept(sock->socket, NULL, NULL);
    break;
  }
  default: {
    printf("BAD ACCEPT.\n");
    break;
  }
  }

  return sock->new_socket > 0;
}

// any
void ssock_recv (ssock_t *sock) {
  assert(sock != NULL);

  recv(sock->new_socket, sock->buffer, sock->bufsize, 0);
}

// any
void ssock_write (ssock_t *sock, char *msg) {
  assert(sock != NULL);
  assert(msg != NULL);

  write(sock->new_socket, msg, strlen(msg));
}

// any
void ssock_close (ssock_t *sock, int which) {
  assert(sock != NULL);

  if (which == 1)
    close(sock->new_socket);
  else if (which == 2) {
    close(sock->socket);
    free(sock->buffer);
  }
}
