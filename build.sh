#!/bin/sh

set -xe
CFLAGS="-Wall -Wextra `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw"

gcc $CFLAGS -o libplug.so -fPIC -shared plug.c $LIBS
gcc $CFLAGS -o musializer  main.c $LIBS -L.
