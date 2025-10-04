#ifndef PLUG_H_
#define PLUG_H_
#include<math.h>
#include<complex.h>
#include<raylib.h>
#include<assert.h>

#define N 256

typedef struct {	Music music;
} Plug;

typedef void (*plug_hello_t)(void);
typedef void (*plug_init_t)(Plug *plug);
typedef void (*plug_update_t)(Plug *plug);

#endif
