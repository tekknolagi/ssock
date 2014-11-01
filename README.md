# ssock - simple sockets library

## What?

I found sockets really confusing, so I wanted to make something to make getting
started easier. This is only a simple wrapper around the basic function calls.

## Does it work?

Surprisingly, yes! I tested with a browser and socat. This allows for arbitrary
responses with functions that return `char *`.

Careful: I have no idea what machines or compilers this works with. OS X 10.10
on Macbook Air right now with gcc/clang. Tested on Arch Linux with gcc.

## How do I work it?

For AF_INET, just point a browser to `localhost:PORT`, where PORT is whatever
you indicated.

For AF_UNIX, just [download and] run `socat` like so: `socat - UNIX-CONNECT:my.sock`.
`socat` is the bee's knees.

## How's the performance?

It takes up some more space for the structs and has some other function call
overhead. Probably shouldn't use it for anything mission critical.

## Memory leaks, etc?

There are no tests and I am not sure that it is leak-proof. Yosemite KILLED
valgrind and this Macbook Air is my main dev machine. :(
