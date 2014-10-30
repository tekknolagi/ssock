#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "ssock.h"
#include "sserv.h"

sserv_status_t sserv_init (ssock_t *sock, sserv_settings_t settings) {
  assert(sock != NULL);

  if (!ssock_init(sock, settings.bufsize))
    return SSERV_INITF;

  if (!ssock_bind(sock, settings.port))
    return SSERV_BINDF;

  if (!ssock_listen(sock, settings.backlog))
    return SSERV_LISTENF;

  return SSERV_OK;
}

sserv_status_t sserv_serve (ssock_t *sock, sserv_func_t f) {
  assert(sock != NULL);
  assert(f != NULL);

  while (true) {
    if (!ssock_accept(sock))
      return SSERV_ACCEPTF;

    ssock_recv(sock);

    char *resp = f(sock);
    ssock_write(sock, resp ? resp : "no.\n");
    ssock_close(sock, 1);
  }

  ssock_close(sock, 2);
  return SSERV_OK;
}
