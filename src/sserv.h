#ifndef SSERV_H
#define SSERV_H

#include "ssock.h"

// keep track of errors in the simple serv
typedef enum {
  SSERV_OK = 0,
  SSERV_INITF,
  SSERV_BINDF,
  SSERV_LISTENF,
  SSERV_ACCEPTF,
  SSERV_RESPONDF
} sserv_status_t;

sserv_status_t sserv_init (ssock_t *);
sserv_status_t sserv_serve (ssock_t *);

#endif
