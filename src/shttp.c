#include <string.h>
#include <assert.h>

#include "shttp.h"

static size_t first_newline (char *);

/*
Sample:

GET / HTTP/1.1
Host: localhost:15002
Connection: keep-alive
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/40.0.2177.0 Safari/537.36
DNT: 1
Accept-Encoding: gzip, deflate, sdch
*/

static size_t first_newline (char *buf) {
  if (!buf) return -1;

  size_t i, len = strlen(buf);

  for (i = 0; buf[i] != '\n'; i++)
    if (i >= len) return -1;

  return i;
}

char *shttp_process_request (ssock_t *sock) {
  if (!sock) return NULL;

  size_t first_nl = first_newline(sock->buffer);
  if (first_nl == -1) return NULL;

  char *method = strnstr(sock->buffer, "GET", first_nl);
  if (!method) return NULL;

  return method;
}
