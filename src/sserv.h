#ifndef SSERV_H
#define SSERV_H

#include "ssock.h"

typedef enum {
  SSERV_OK = 0,
  SSERV_INITF,
  SSERV_BINDF,
  SSERV_LISTENF,
  SSERV_ACCEPTF,
  SSERV_RESPONDF
} sserv_status_t;

typedef struct sserv_settings_s {
  int port, bufsize, backlog;
} sserv_settings_t;

typedef char * (* sserv_func_t)(ssock_t *);

sserv_status_t sserv_init (ssock_t *, sserv_settings_t);
sserv_status_t sserv_serve (ssock_t *, sserv_func_t);

#endif
