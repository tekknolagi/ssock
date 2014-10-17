#ifndef SHTTP_H
#define SHTTP_H

#include "sserv.h"

typedef enum {
  SHTTP_GET,
  SHTTP_POST,
  SHTTP_UNDEF,
} shttp_method_t;

typedef enum {
  SHTTP_1_0,
  SHTTP_1_1,
  SHTTP_2_0
} shttp_version_t;

typedef struct shttp_ctx_s {
  char *request;

  shttp_method_t method;
  char *route;
  shttp_version_t version;
  
  char *host;
  int port;
} shttp_ctx_t;

shttp_ctx_t *shttp_process_request (ssock_t *);
char *shttp_respond (shttp_ctx_t *);
void shttp_destroy_ctx (shttp_ctx_t *);

#endif
