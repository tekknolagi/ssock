#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "ssock/ssock.h"

void interrupt_handler (int sig) {
  puts("Interrupted.");
  exit(5);
}

int main () {
  ssock_t sock;
  if (!ssock_init(&sock, 1024)) return 1;
  if (!ssock_bind(&sock, 15002)) return 2;
  if (!ssock_listen(&sock)) return 3;

  signal(SIGINT, interrupt_handler);

  while (true) {
    if (!ssock_accept(&sock)) return 4;

    ssock_recv(&sock);
    ssock_write(&sock, "hello, world!\n\0");
    ssock_close(&sock, 1);
  }

  ssock_close(&sock, 2);
}
