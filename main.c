#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "ssock/ssock.h"
#include "ssock/sserv.h"

void interrupt_handler (int sig) {
  puts("Interrupted.");
  exit(5);
}

char *cat (ssock_t *sock) {
  return sock->buffer;
}

char *yourip (ssock_t *sock) {
  inet_ntop(AF_INET, &(sock->address.sin_addr.s_addr),
	    sock->buffer, INET_ADDRSTRLEN);

  return sock->buffer;
}

int main () {
  signal(SIGINT, interrupt_handler);

  ssock_t sock;
  if (sserv_init(&sock, 15002) != SSERV_OK) return 1;
  if (sserv_serve(&sock, cat) != SSERV_OK) return 2;

  return 0;
}
