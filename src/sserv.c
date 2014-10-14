#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "ssock.h"
#include "sserv.h"

sserv_status_t sserv_init (ssock_t * sock, int port) {
  if (!ssock_init(sock, 1024))
    return SSERV_INITF;

  if (!ssock_bind(sock, port))
    return SSERV_BINDF;

  if (!ssock_listen(sock))
    return SSERV_LISTENF;

  return SSERV_OK;
}

sserv_status_t sserv_serve (ssock_t * sock, sserv_func_t f) {
  while (true) {
    if (!ssock_accept(sock))
      return SSERV_ACCEPTF;

    ssock_recv(sock);
    ssock_write(sock, f(sock));
    ssock_close(sock, 1);
  }

  ssock_close(sock, 2);
  return SSERV_OK;
}
