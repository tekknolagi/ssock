#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "ssock.h"
#include "sserv.h"

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

  while (true) {
    if (!ssock_accept(sock))
      return SSERV_ACCEPTF;

    ssize_t rcved = ssock_recv(sock);
    printf("%lu bytes received.\n", rcved);

    char *resp = sock->f(sock);
    // TODO: FIX
    // Function returned NULL for some reason...
    // Not sure how to address this.
    ssize_t written = ssock_write(sock, resp ? resp : "No.\n");
    printf("%lu bytes written.\n", written);
    printf("---\n");

    ssock_close(sock, 1);
  }

  // And I need to figure out why there are two sockets.
  ssock_close(sock, 2);
  return SSERV_OK;
}
