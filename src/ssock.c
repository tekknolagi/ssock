#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

#include "ssock.h"

// This function is sock-domain agnostic.
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

// This function is NOT sock-domain agnostic. I wish I could make it so.
bool ssock_bind (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->domain) {
  case AF_INET: {
    assert(sock->settings.inet.port > 0);

    struct sockaddr_in *sa = (struct sockaddr_in *) &sock->settings.address;
    sa->sin_family = AF_INET;
    // translate the IP from string form into int form
    inet_pton(AF_INET, sock->settings.inet.addr, &(sa->sin_addr));
    sa->sin_port = htons(sock->settings.inet.port);

    return bind(sock->socket, (struct sockaddr *) sa, sizeof *sa) == 0;
  }
  case AF_UNIX: {
    assert(sock->settings.unix.path != NULL);

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

// This function is sock-domain agnostic.
bool ssock_listen (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->backlog > 0);

  return listen(sock->socket, sock->backlog) == 0;
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

// This function is NOT sock-domain agnostic. I wish I could make it so.
bool ssock_accept (ssock_t *sock) {
  assert(sock != NULL);

  switch (sock->domain) {
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
    puts("BAD SOCK DOMAIN.");
    return false;
  }
  }

  return sock->new_socket > 0;
}

// This function is sock-domain agnostic.
ssize_t ssock_recv (ssock_t *sock) {
  assert(sock != NULL);

  return recv(sock->new_socket, sock->buffer, sock->bufsize, 0);
}

// This function is sock-domain agnostic.
ssize_t ssock_write (ssock_t *sock, char *msg) {
  assert(sock != NULL);
  assert(msg != NULL);

  return write(sock->new_socket, msg, strlen(msg));
}

// This function is sock-domain agnostic.
ssize_t ssock_client_recv (ssock_t *sock) {
  assert(sock != NULL);

  return recv(sock->socket, sock->buffer, sock->bufsize, 0);
}

// This function is sock-domain agnostic.
ssize_t ssock_client_write (ssock_t *sock, char *msg) {
  assert(sock != NULL);

  return write(sock->socket, msg, strlen(msg));
}

// This function is sock-domain agnostic.
void ssock_close (ssock_t *sock, int which) {
  assert(sock != NULL);

  if (which == 1)
    close(sock->new_socket);
  else if (which == 2) {
    close(sock->socket);
    free(sock->buffer);
  }
}
