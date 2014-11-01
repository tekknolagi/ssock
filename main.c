#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

#include "src/ssock.h"
#include "src/sserv.h"

void interrupt_handler (int sig) {
  puts("Interrupted.");
  exit(5);
}

char *cat (void *sock) {
  assert(sock != NULL);

  // cast because otherwise we have this weird circular
  // fustercluck in the header
  // - too tired to fix right now
  ssock_t *s = (ssock_t *) sock;
  return s->buffer;
}

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("Needs one argument: sock path.\n");
    exit(1);
  }

  signal(SIGINT, interrupt_handler);

  ssock_t sock = {
    .type = AF_UNIX,
    .backlog = 10,
    .bufsize = 1024,
    .f = cat,

    .settings.af_unix = {
      .path = argv[1]
    }
  };

  if (sserv_init(&sock) != SSERV_OK) return 1;
  if (sserv_serve(&sock) != SSERV_OK) return 2;

  return 0;
}
