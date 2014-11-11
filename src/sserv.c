#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "ssock.h"
#include "sserv.h"

static void sserv_debug (size_t, size_t);

// Simple error-checking and wrapper for ssock.
sserv_status_t sserv_init (ssock_t *sock) {
  assert(sock != NULL);

  if (!ssock_init(sock)) return SSERV_INITF;
  if (!ssock_bind(sock)) return SSERV_BINDF;
  if (!ssock_listen(sock)) return SSERV_LISTENF;

  return SSERV_OK;
}

// Serve one page at a time. Could be pthreaded.... maybe.
sserv_status_t sserv_serve (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->f != NULL);

  while (sock->serving) {
    if (!ssock_accept(sock))
      return SSERV_ACCEPTF;

    ssize_t received = ssock_recv(sock);

    char *resp = sock->f(sock);
    // TODO: FIX
    // Function returned NULL for some reason...
    // Not sure how to address this.
    ssize_t written = ssock_write(sock, resp ? resp : "No.\n");
    sserv_debug(received, written);

    ssock_close(sock, 1);
  }

  // And I need to figure out why there are two sockets.
  ssock_close(sock, 2);
  return SSERV_OK;
}

static void sserv_debug (size_t received, size_t written) {
    printf("%lu bytes received.\n", received);
    printf("%lu bytes written.\n", written);
    printf("---\n");
}
