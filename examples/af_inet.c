#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#include <ssock/ssock.h>
#include <ssock/sserv.h>

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
  // have to debug Ctrl-C issue
  // sometimes it does not exit
  signal(SIGINT, interrupt_handler);

  ssock_t sock = {
    .type = AF_INET,
    .backlog = 10,
    .bufsize = 1024,
    .f = cat,

    .settings.af_inet = {
      .port = 15000
    }
  };

  // some minor diagnostics about why it died
  if (sserv_init(&sock) != SSERV_OK) return 1;
  if (sserv_serve(&sock) != SSERV_OK) return 2;

  return 0;
}
