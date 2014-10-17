#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "shttp.h"

/*
GET / HTTP/1.1
Host: localhost:15002
Connection: keep-alive
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_0) AppleWebKit/537.36
  (KHTML, like Gecko) Chrome/40.0.2177.0 Safari/537.36
DNT: 1
Accept-Encoding: gzip, deflate, sdch
*/

size_t first_newline (char *buf) {
  if (!buf) return -1;

  size_t i;
  for (i = 0; buf[i] != 0 && buf[i] != '\n'; i++);
  return i;
}

shttp_ctx_t *shttp_process_request (ssock_t *sock) {
  if (!sock) return NULL;

  char *nextspace;

  shttp_ctx_t *ctx = malloc(sizeof *ctx);
  if (!ctx) return NULL;

  ctx->request = sock->buffer;

  size_t first_nl = first_newline(sock->buffer);
  if (first_nl == -1) return NULL;

  char *method = strnstr(sock->buffer, "GET", first_nl);
  if (!method) return NULL;

  ctx->method = SHTTP_GET;

  nextspace = strstr(method, " ");
  if (!nextspace) return NULL;

  char *route = strstr(nextspace, "/");
  if (!route) return NULL;

  nextspace = strstr(route, " ");
  if (!nextspace) return NULL;

  long routel = nextspace - route;
  ctx->route = calloc(routel, sizeof *ctx->route);
  if (!ctx->route) return NULL;

  strncpy(ctx->route, route, routel);

  // placeholder
  ctx->host = malloc(1);
  if (!ctx->host) return NULL;

  return ctx;
}

void shttp_destroy_ctx (shttp_ctx_t *ctx) {
  free(ctx->route);
  free(ctx->host);
  free(ctx);
}
