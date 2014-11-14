#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#include <ssock/ssock.h>
#include <ssock/sserv.h>

char *cat (ssock_t *);

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("Needs one argument: sock path.\n");
    exit(1);
  }

  ssock_t sock = {
    .domain = AF_UNIX,
    .type = SOCK_STREAM,
    .backlog = 10,
    .bufsize = 1024,
    .f = cat,

    .settings.unix.path = argv[1]
  };

  // some minor diagnostics about why it died
  if (sserv_init(&sock) != SSERV_OK) return 1;
  if (sserv_serve(&sock) != SSERV_OK) return 2;

  return 0;
}

char *cat (ssock_t *sock) {
  assert(sock != NULL);

  sock->serving = false;
  return sock->buffer;
}
