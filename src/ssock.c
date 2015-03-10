#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

#include "ssock.h"

bool ssock_init (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->bufsize > 0);

  sock->socket = socket(sock->domain, sock->type, 0);
  bool success = sock->socket > 0;

  if (success) {
    sock->buffer = calloc(sock->bufsize, sizeof *sock->buffer);
    if (!sock->buffer) return false;
  }

  sock->serving = true;
  return success;
}

bool ssock_bind (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->domain) {
  case AF_INET: {
    assert(sock->settings.inet.port > 0);

    struct sockaddr_in *sa = (struct sockaddr_in *) &sock->settings.address;
    sa->sin_family = AF_INET;
    // translate the IP from string form into int form
    inet_pton(AF_INET, sock->settings.inet.addr, &(sa->sin_addr));
    // translate port from host to network byte order
    sa->sin_port = htons(sock->settings.inet.port);

    int reuse = 1;
    setsockopt(sock->socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);
    return bind(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  case AF_UNIX: {
    assert(sock->settings.unix.path != NULL);

    // truncate at 108 chars
    size_t len_path = strlen(sock->settings.unix.path);
    size_t max_length = (len_path > 108) ? 108 : len_path;

    struct sockaddr_un *sa = (struct sockaddr_un *) &sock->settings.address;
    sa->sun_family = AF_UNIX;
    strncpy(sa->sun_path, sock->settings.unix.path, max_length);
    unlink(sa->sun_path);

    return bind(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  default: {
    puts("BAD SOCK DOMAIN.");
    return false;
  }
  }
}

bool ssock_listen (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->backlog > 0);

  if (sock->type == SOCK_STREAM)
    return listen(sock->socket, sock->backlog) == 0;

  return true;
}

bool ssock_connect (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->domain) {
  case AF_INET: {
    assert(sock->settings.inet.port > 0);

    struct sockaddr_in *sa = (struct sockaddr_in *) &sock->settings.address;
    sa->sin_family = AF_INET;
    // translate the IP from string form into int form
    inet_pton(AF_INET, sock->settings.inet.addr, &(sa->sin_addr));
    // translate port from host to network byte order
    sa->sin_port = htons(sock->settings.inet.port);

    return connect(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  case AF_UNIX: {
    assert(sock->settings.unix.path != NULL);

    size_t len_path = strlen(sock->settings.unix.path);
    size_t max_length = (len_path > 108) ? 108 : len_path;

    struct sockaddr_un *sa = (struct sockaddr_un *) &sock->settings.address;
    sa->sun_family = AF_UNIX;
    strncpy(sa->sun_path, sock->settings.unix.path, max_length);

    return connect(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  default: {
    puts("BAD SOCK DOMAIN.");
    return false;
  }
  }
}

bool ssock_accept (ssock_t *sock) {
  assert(sock != NULL);

  /*
    Turns out this dramatically shortens the code. If AF_INET, accept() needs
    an address and address length, but otherwise not. I used to have a switch
    but that looked hideous.
  */
  struct sockaddr *sa =
    (sock->domain == AF_INET)
    ? &sock->settings.address
    : NULL;

  sock->new_socket =
    accept(sock->socket, sa, sa ? &sock->settings.address_len : NULL);

  return sock->new_socket > 0;
}

ssize_t ssock_recv (ssock_t *sock) {
  assert(sock != NULL);

  return recv(sock->new_socket, sock->buffer, sock->bufsize, 0);
}

ssize_t ssock_write (ssock_t *sock, char *msg) {
  assert(sock != NULL);
  assert(msg != NULL);

  return write(sock->new_socket, msg, strlen(msg));
}

ssize_t ssock_client_recv (ssock_t *sock) {
  assert(sock != NULL);

  return recv(sock->socket, sock->buffer, sock->bufsize, 0);
}

ssize_t ssock_client_write (ssock_t *sock, char *msg) {
  assert(sock != NULL);

  return write(sock->socket, msg, strlen(msg));
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
