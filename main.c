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

char *yourip (ssock_t *sock) {
  snprintf(sock->buffer, 17, "%d.%d.%d.%d\n",
	   (int) (sock->address.sin_addr.s_addr & 0xFF),
	   (int) ((sock->address.sin_addr.s_addr & 0xFF00) >> 8),
	   (int) ((sock->address.sin_addr.s_addr & 0xFF0000) >> 16),
	   (int) ((sock->address.sin_addr.s_addr & 0xFF000000) >> 24));

  return sock->buffer;
}

int main () {
  signal(SIGINT, interrupt_handler);

  ssock_t sock;
  if (sserv_init(&sock, 15002) != SSERV_OK) return 1;
  if (sserv_serve(&sock, yourip) != SSERV_OK) return 2;

  return 0;
}
