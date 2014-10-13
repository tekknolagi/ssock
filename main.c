#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "ssock.h"

#define p(x) puts(#x)

volatile sig_atomic_t running = 1;

void interrupt_handler (int sig) {
  p(interrupted with\n);
  printf("%d\n", sig);
  running = 0;
  exit(5);
}

int main () {
  sock_t sock;
  if (!sock_init(&sock, 1024)) return 1;
  p(init);
  if (!sock_bind(&sock, 15002)) return 2;
  p(bound);
  if (!sock_listen(&sock)) return 3;
  p(listening);

  signal(SIGINT, interrupt_handler);

  while (running) {
    if (!sock_accept(&sock)) return 4;
    p(accept);

    sock_recv(&sock);
    printf("%s\n", sock.buffer);
    sock_write(&sock, "hello, world!\n\0");
    sock_close(&sock, 1);
  }

  sock_close(&sock, 2);
}
