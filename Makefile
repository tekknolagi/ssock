CC ?= gcc
PREFIX = /usr/local
NDEBUG ?= 
FLTO ?= 
CFLAGS ?= -g3 -std=c99 -O3 $(FLTO) -Wall -Werror -Wno-unused-variable \
	-Wno-format-security $(NDEBUG)
SRCS = src/ssock.c src/sserv.c src/sclient.c
OBJS = *.o
PROG = ssock.out
PROJNAME = ssock
ARCHIVE = libssock.a

all: build clean_objs

build:
	$(CC) -c $(CFLAGS) $(SRCS)
	ar cr $(ARCHIVE) $(OBJS)
#	$(CC) $(CFLAGS) main.c $(ARCHIVE) -o $(PROG)

uninstall:
	rm -r $(DESTDIR)$(PREFIX)/include/$(PROJNAME)
	rm $(DESTDIR)$(PREFIX)/lib/$(ARCHIVE)

install:
	test -d ${DESTDIR}${PREFIX}/include/$(PROJNAME) || \
		mkdir -p ${DESTDIR}${PREFIX}/include/$(PROJNAME)
	cp src/*.h $(DESTDIR)$(PREFIX)/include/$(PROJNAME)
	cp $(ARCHIVE) $(DESTDIR)$(PREFIX)/lib

clean_libs:
	find . -name "*.a"	\
	| xargs rm -f

clean_objs:
	find . -name "*.o"	\
	-o -name "*.dSYM"	\
	 | xargs rm -rf

clean_outs:
	find . -name "*.out"	\
	| xargs rm -f

clean: clean_objs
	rm -f $(PROG) $(ARCHIVE)
