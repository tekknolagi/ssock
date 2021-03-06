#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#include <ssock/ssock.h>
#include <ssock/sserv.h>

void interrupt_handler (int);
char *cat (ssock_t *);

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("Needs one argument: sock path.\n");
    exit(1);
  }

  // have to debug Ctrl-C issue
  // sometimes it does not exit
  signal(SIGINT, interrupt_handler);

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

void interrupt_handler (int sig) {
  puts("Interrupted.");
  exit(5);
}

char *cat (ssock_t *sock) {
  assert(sock != NULL);

  return sock->buffer;
}
