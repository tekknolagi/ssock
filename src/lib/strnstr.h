#ifndef STRNSTR_H
#define STRNSTR_H

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)strstr.c  8.1 (Berkeley) 6/4/93";
#endif /* LIBC_SCCS and not lint */

char *strnstr (const char *, const char *, size_t);

#endif
