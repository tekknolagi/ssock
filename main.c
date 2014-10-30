#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "ssock/ssock.h"
#include "ssock/sserv.h"
#include "ssock/shttp.h"

void interrupt_handler (int sig) {
  puts("Interrupted.");
  exit(5);
}

char *cat (ssock_t *sock) {
  if (!sock) return NULL;

  return sock->buffer;
}

int main () {
  signal(SIGINT, interrupt_handler);

  ssock_t sock;
  if (sserv_init(&sock, 15000, 1024, 10) != SSERV_OK) return 1;
  if (sserv_serve(&sock, cat) != SSERV_OK) return 2;

  return 0;
}
