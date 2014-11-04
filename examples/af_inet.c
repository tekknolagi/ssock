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

char *cat (ssock_t *sock) {
  assert(sock != NULL);

  return sock->buffer;
}

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("Needs one argument: port num.\n");
    exit(1);
  }

  // have to debug Ctrl-C issue
  // sometimes it does not exit
  signal(SIGINT, interrupt_handler);

  char *endptr;

  ssock_t sock = {
    .type = AF_INET,
    .backlog = 10,
    .bufsize = 1024,
    .f = cat,

    .settings.af_inet = {
      .port = strtol(argv[1], &endptr, 10)
    }
  };

  // some minor diagnostics about why it died
  if (sserv_init(&sock) != SSERV_OK) return 1;
  if (sserv_serve(&sock) != SSERV_OK) return 2;

  return 0;
}
