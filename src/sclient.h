#ifndef SCLIENT_H
#define SCLIENT_H

#include "ssock.h"

// keep track of errors in the simple serv
typedef enum {
  SCLIENT_OK = 0,
  SCLIENT_INITF,
  SCLIENT_CONNECTF,
  SCLIENT_WRITEF,
  SCLIENT_READF
} sclient_status_t;

sclient_status_t sclient_connect (ssock_t *);
sclient_status_t sclient_speak (ssock_t *, char *msg);
void sclient_close (ssock_t *);

#endif
