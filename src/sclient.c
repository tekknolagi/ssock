#include <assert.h>

#include "ssock.h"
#include "sclient.h"

sclient_status_t sclient_connect (ssock_t *sock) {
  assert(sock != NULL);

  if (!ssock_init(sock)) return SCLIENT_INITF;
  if (!ssock_connect(sock)) return SCLIENT_CONNECTF;

  return SCLIENT_OK;
}

sclient_status_t sclient_speak (ssock_t *sock, char *msg) {
  assert(sock != NULL);

  if (!ssock_write(sock, msg)) return SCLIENT_WRITEF;
  if (!ssock_recv(sock)) return SCLIENT_READF;

  return SCLIENT_OK;
}

void sclient_close (ssock_t *sock) {
  assert(sock != NULL);

  ssock_close(sock, 2);
}
