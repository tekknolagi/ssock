#ifndef SHTTP_H
#define SHTTP_H

#include "sserv.h"

/* typedef struct shttp_ctx_s { */
/*   char *body; */
/* } shttp_ctx_t; */

char *shttp_process_request (ssock_t *);

#endif
