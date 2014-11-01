#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "ssock.h"
#include "sserv.h"

sserv_status_t sserv_init (ssock_t *sock) {
  assert(sock != NULL);

  if (!ssock_init(sock))
    return SSERV_INITF;

  if (!ssock_bind(sock))
    return SSERV_BINDF;

  if (!ssock_listen(sock))
    return SSERV_LISTENF;

  return SSERV_OK;
}

sserv_status_t sserv_serve (ssock_t *sock) {
  assert(sock != NULL);
  assert(sock->settings.af_inet.f != NULL);

  while (true) {
    if (!ssock_accept(sock))
      return SSERV_ACCEPTF;

    ssock_recv(sock);

    char *resp = sock->settings.af_inet.f(sock);
    ssock_write(sock, resp ? resp : "no.\n");
    ssock_close(sock, 1);
  }

  ssock_close(sock, 2);
  return SSERV_OK;
}
