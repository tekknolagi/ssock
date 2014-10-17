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

/* char *cat (ssock_t *sock) { */
/*   if (!sock) return NULL; */

/*   return sock->buffer; */
/* } */

/* char *yourip (ssock_t *sock) { */
/*   if (!sock) return NULL; */

/*   inet_ntop(AF_INET, &(sock->address.sin_addr.s_addr), */
/* 	    sock->buffer, INET_ADDRSTRLEN); */

/*   return sock->buffer; */
/* } */

char *http (ssock_t *sock) {
  shttp_ctx_t *ctx = shttp_process_request(sock);
  if (!ctx) return NULL;

  sock->response = calloc(strlen(ctx->route), sizeof *sock->response);
  if (!sock->response) return NULL;

  strncpy(sock->response, ctx->route, strlen(ctx->route));
  shttp_destroy_ctx(ctx);

  return sock->response;
}

int main () {
  signal(SIGINT, interrupt_handler);

  ssock_t sock;
  if (sserv_init(&sock, 15000) != SSERV_OK) return 1;
  if (sserv_serve(&sock, http) != SSERV_OK) return 2;

  return 0;
}
