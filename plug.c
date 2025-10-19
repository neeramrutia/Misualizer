#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "plug.h"
#include <string.h>
#define N (1<<13)

typedef struct{
	float left;
	float right;
} Frame;

float in[N];
float complex out[N];
void callback(void *bufferdata, unsigned int frames){
	if(frames > N) frames = N;

	Frame *fs = bufferdata;
	for(size_t i=0;i<frames;i++){
		memmove(in , in+1 , (N-1)*sizeof(in[0]));
		in[N-1] = fs[i].left;
	}
	
}
Color get_random_color(){
	Color color[10] = {WHITE,GREEN,BLUE,YELLOW,RED,PURPLE,GOLD,ORANGE,PINK,VIOLET};
	int random = GetRandomValue(1,  10);
	return color[random];
	
}
void fft(float in[], size_t stride, float complex out[],size_t n){
	assert(n > 0);
	if(n==1){
		out[0] = in[0];
		return;
	}
	fft(in,stride*2,out,n/2);
	fft(in +stride, stride*2,out +n/2,n/2);
	for(size_t k=0;k<n/2;k++){
		float t = (float)k/n;
		float complex v = cexp(-2*I*PI*t)*out[k+n/2];
		float complex e = out[k];
		out[k] = e+v;
		out[k+n/2] = e-v;
	}
}

float amp(float complex z){
	float a = fabsf(crealf(z));
	float b = fabsf(cimagf(z));
	if(a<b) return b;
	return a; 	
}
void plug_hello(void){
	printf("hello from plug");
}

 void plug_init(Plug *plug){

 	plug->music =LoadMusicStream("../Travis_Scott_-_HIGHEST_IN_THE_ROOM_(mp3.pm).mp3");
 	PlayMusicStream(plug->music);
 	
 	AttachAudioStreamProcessor(plug->music.stream, callback);
 	
 }


void  plug_pre_reload(Plug *plug){
	DetachAudioStreamProcessor(plug->music.stream, callback);
}
void plug_post_reload(Plug *plug){
	AttachAudioStreamProcessor(plug->music.stream, callback);
}
 
 void plug_update(Plug *plug){
 	UpdateMusicStream(plug->music);

		if(IsKeyPressed(KEY_SPACE)){
			if(IsMusicStreamPlaying(plug->music)){
				PauseMusicStream(plug->music);
			}else{
				ResumeMusicStream(plug->music);
			}
		}

		int w = GetRenderWidth();
		int h = GetRenderHeight();

		BeginDrawing();
		ClearBackground(BLACK);
		fft(in,1,out,N); 
			float max_amp = 0.0f;
			for(size_t i=0;i<N;i++){
				float a = amp(out[i]); 
				if(max_amp < a) max_amp = a;
			}
		//float cellWidth = 2; 

		float step = 1.06;
		    size_t m = 0;
		    for (float f = 20.0f; (size_t) f < N; f *= step) {
		        m += 1;
		    }
		
		    float cell_width = (float)w/m;
		    m = 0;
		    for (float f = 20.0f; (size_t) f < N; f *= step) {
		        float f1 = f*step;
		        float a = 0.0f;
		        for (size_t q = (size_t) f; q < N && q < (size_t) f1; ++q) {
		            a += amp(out[q]);
		        }
		        a /= (size_t) f1 - (size_t) f + 1;
		        float t = a/max_amp;
		        DrawRectangle(m*cell_width, h/2 - h/2*t, cell_width, h/2*t, WHITE);
		        m += 1;
		    }
	/*	float cellWidth =  (float)w/N;
		printf("%f" , cellWidth);
		// drawing one frame of a signal
		for(int i = 0;i<N;i++){
				float t = amp(out[i])/max_amp;
				//Color c = ColorAlphaBlend(RED , ColorAlpha(GREEN , t) , WHITE);
				DrawRectangle(i*ceil(cellWidth),h/2-h/2*t,ceil(cellWidth),h/2*t,get_random_color());
			
		}
		*/
		EndDrawing(); 
 	
 }
