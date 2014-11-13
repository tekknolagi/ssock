#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#include <ssock/ssock.h>
#include <ssock/sclient.h>

int main (int argc, char **argv) {
  if (argc < 3) {
    printf("Needs two argument: addr, port num.\n");
    exit(1);
  }

  ssock_t sock = {
    .type = AF_INET,
    .bufsize = 1024,

    .settings.inet.addr = argv[1],
    .settings.inet.port = strtol(argv[2], &endptr, 10)
  };

  // some minor diagnostics about why it died
  if (sclient_connect(&sock) != SCLIENT_OK) return 1;
  if (sclient_speak(&sock, "boo") != SCLIENT_OK) return 2;

  printf("--%s--\n", sock.buffer);

  sclient_close(&sock);

  return 0;
}
