#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#include <ssock/ssock.h>
#include <ssock/sclient.h>

int main (int argc, char **argv) {
  if (argc < 2) {
    printf("Needs one argument: sock path.\n");
    exit(1);
  }

  ssock_t sock = {
    .domain = AF_UNIX,
    .type = SOCK_STREAM,
    .bufsize = 1024,

    .settings.unix.path = argv[1]
  };

  // some minor diagnostics about why it died
  if (sclient_connect(&sock) != SCLIENT_OK) return 1;
  if (sclient_speak(&sock, "boo") != SCLIENT_OK) return 2;

  printf("--%s--\n", sock.buffer);

  sclient_close(&sock);

  return 0;
}
