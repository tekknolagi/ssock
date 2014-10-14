#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "ssock/ssock.h"
#include "ssock/sserv.h"

void interrupt_handler (int sig) {
  puts("Interrupted.");
  exit(5);
}

char *mirror (ssock_t *sock) {
  return sock->buffer;
}

int main () {
  signal(SIGINT, interrupt_handler);

  ssock_t sock;
  if (sserv_init(&sock, 15002) != SSERV_OK) return 1;
  if (sserv_serve(&sock, mirror) != SSERV_OK) return 2;

  return 0;
}
