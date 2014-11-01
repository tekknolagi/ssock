#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "src/ssock.h"
#include "src/sserv.h"

void interrupt_handler (int sig) {
  puts("Interrupted.");
  exit(5);
}

char *cat (void *sock) {
  if (!sock) return NULL;

  ssock_t *s = (ssock_t *) sock;
  return s->buffer;
}

int main () {
  signal(SIGINT, interrupt_handler);

  ssock_t sock = {
    .type = AF_INET,
    .backlog = 10,
    .settings.af_inet = {
	.port = 15000,
	.bufsize = 1024,
	.f = cat
    }
  };

  if (sserv_init(&sock) != SSERV_OK) return 1;
  if (sserv_serve(&sock) != SSERV_OK) return 2;

  return 0;
}
