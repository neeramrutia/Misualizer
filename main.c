#include <raylib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include <dlfcn.h>
#include "plug.h"

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

typedef struct{
	float left;
	float right;
} Frame;






const char *libplug_file_name = "libplug.so";
void *libplug = NULL;
plug_hello_t plug_hello = NULL;
plug_init_t plug_init = NULL;
plug_update_t plug_update = NULL;
plug_pre_reload_t plug_pre_reload = NULL;
plug_post_reload_t plug_post_reload = NULL;
Plug plug = {0};
bool reload_libplug(void){
	if(libplug != NULL)  dlclose(libplug);
	libplug = dlopen(libplug_file_name, RTLD_NOW);
	if(libplug == NULL){
		fprintf(stderr , "ERR : could not load %s : %s" , libplug_file_name, dlerror());
		return false;
	}
	plug_hello = dlsym(libplug, "plug_hello");
	if(plug_hello == NULL){
		fprintf(stderr, "Couldn't find plug_hello symbol in %s : %s" , libplug_file_name, dlerror());
		return false;
	}
	plug_init = dlsym(libplug, "plug_init");
	if(plug_init == NULL){
		fprintf(stderr, "Couldn't find plug_init symbol in %s : %s" , libplug_file_name, dlerror());
		return false;
	}
	plug_update = dlsym(libplug, "plug_update");
	if(plug_update == NULL){
		fprintf(stderr, "Couldn't find plug_update symbol in %s : %s" , libplug_file_name, dlerror());
		return false;
	}
	plug_pre_reload = dlsym(libplug, "plug_pre_reload");
		if(plug_pre_reload == NULL){
			fprintf(stderr, "Couldn't find plug_pre_reload symbol in %s : %s" , libplug_file_name, dlerror());
			return false;
		}
	plug_post_reload = dlsym(libplug, "plug_post_reload");
		if(plug_post_reload == NULL){
			fprintf(stderr, "Couldn't find plug_post_reload symbol in %s : %s" , libplug_file_name, dlerror());
			return false;
		}
	return true;		
}

int main(void)
{

	
	if(!reload_libplug()) return 1;
	
	plug_hello();

	InitWindow(1200,900,"Musializer");
	SetTargetFPS(60);
	InitAudioDevice();
	

	plug_init(&plug);
	while(!WindowShouldClose()){
		if(IsKeyPressed(KEY_R)){
			plug_pre_reload(&plug);
				if(!reload_libplug()) return 1;
			plug_post_reload(&plug);	
		}
		plug_update(&plug);
	}
	
    return 0;
}
