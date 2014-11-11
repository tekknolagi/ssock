#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ssock.h"

// This function is sock-type agnostic.
bool ssock_init (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->bufsize > 0);

  sock->socket = socket(sock->type, SOCK_STREAM, 0);
  bool success = sock->socket > 0;

  if (success) {
    sock->buffer = calloc(sock->bufsize, sizeof *sock->buffer);
    if (!sock->buffer) return false;
  }

  sock->serving = true;
  return success;
}

// This function is NOT sock-type agnostic. I wish I could make it so.
bool ssock_bind (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->type) {
  case AF_INET: {
    assert(sock->settings.port > 0);

    struct sockaddr_in *sa = (struct sockaddr_in *) &sock->settings.address;
    sa->sin_family = AF_INET;
    sa->sin_addr.s_addr = INADDR_ANY;
    sa->sin_port = htons(sock->settings.port);

    return bind(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  case AF_UNIX: {
    assert(sock->settings.path != NULL);
    size_t len_path = strlen(sock->settings.path);
    size_t max_length = (len_path > 108) ? 108 : len_path;

    struct sockaddr_un *sa = (struct sockaddr_un *) &sock->settings.address;
    sa->sun_family = AF_UNIX;
    strncpy(sa->sun_path, sock->settings.path, max_length);
    unlink(sa->sun_path);

    return bind(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  default: {
    puts("BAD SOCK TYPE.");
    return false;
  }
  }
}

// This function is sock-type agnostic.
bool ssock_listen (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->backlog > 0);

  return listen(sock->socket, sock->backlog) == 0;
}

bool ssock_connect (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->type) {
  case AF_INET: {
    assert(sock->settings.port > 0);

    struct sockaddr_in *sa = (struct sockaddr_in *) &sock->settings.address;
    sa->sin_family = AF_INET;
    sa->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa->sin_port = htons(sock->settings.port);

    return connect(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  case AF_UNIX: {
    assert(sock->settings.path != NULL);

    size_t len_path = strlen(sock->settings.path);
    size_t max_length = (len_path > 108) ? 108 : len_path;

    struct sockaddr_un *sa = (struct sockaddr_un *) &sock->settings.address;
    sa->sun_family = AF_UNIX;
    strncpy(sa->sun_path, sock->settings.path, max_length);

    return connect(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  default: {
    puts("BAD SOCK TYPE.");
    return false;
  }
  }
}

// This function is NOT sock-type agnostic. I wish I could make it so.
bool ssock_accept (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->type) {
  case AF_INET: {
    struct sockaddr_in *sa = (struct sockaddr_in *) &sock->settings.address;
    sock->new_socket = accept(sock->socket,
			      (struct sockaddr *) sa,
			      &sock->settings.address_len);
    break;
  }
  case AF_UNIX: {
    sock->new_socket = accept(sock->socket, NULL, NULL);
    break;
  }
  default: {
    puts("BAD SOCK TYPE.");
    return false;
  }
  }

  return sock->new_socket > 0;
}

// This function is sock-type agnostic.
ssize_t ssock_recv (ssock_t *sock) {
  assert(sock != NULL);

  return recv(sock->new_socket, sock->buffer, sock->bufsize, 0);
}

// This function is sock-type agnostic.
ssize_t ssock_write (ssock_t *sock, char *msg) {
  assert(sock != NULL);
  assert(msg != NULL);

  return write(sock->new_socket, msg, strlen(msg));
}

// This function is sock-type agnostic.
ssize_t ssock_read (ssock_t *sock) {
  assert(sock != NULL);

  return read(sock->socket, sock->buffer, sock->bufsize);
}

// This function is sock-type agnostic.
void ssock_close (ssock_t *sock, int which) {
  assert(sock != NULL);

  if (which == 1)
    close(sock->new_socket);
  else if (which == 2) {
    close(sock->socket);
    free(sock->buffer);
  }
}
