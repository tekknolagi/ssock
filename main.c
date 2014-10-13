#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "ssock.h"

#define p(x) puts(#x)

void interrupt_handler (int sig) {
  exit(5);
}

int main () {
  sock_t sock;
  if (!sock_init(&sock, 1024)) return 1;
  if (!sock_bind(&sock, 15002)) return 2;
  if (!sock_listen(&sock)) return 3;

  signal(SIGINT, interrupt_handler);

  while (true) {
    if (!sock_accept(&sock)) return 4;

    sock_recv(&sock);
    sock_write(&sock, "hello, world!\n\0");
    sock_close(&sock, 1);
  }

  sock_close(&sock, 2);
}
