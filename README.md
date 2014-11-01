# ssock - simple sockets library

## What?

I found sockets really confusing, so I wanted to make something to make getting
started easier. This is only a simple wrapper around the basic function calls.

## Does it work?

Surprisingly, yes! I tested with a browser and socat. This allows for arbitrary
responses with functions that return `char *`.

## How's the performance?

It takes up some more space for the structs and has some other function call
overhead. Probably shouldn't use it for anything mission critical.

## Memory leaks, etc?

There are no tests and I am not sure that it is leak-proof. Yosemite KILLED
valgrind and this Macbook Air is my main dev machine. :(
