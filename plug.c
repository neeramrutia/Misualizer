#include <stdio.h>
#include <math.h>
#include "plug.h"


typedef struct{
	float left;
	float right;
} Frame;

float in[N];
float complex out[N];
void callback(void *bufferdata, unsigned int frames){
	if(frames < N) return;

	Frame *fs = bufferdata;
	for(size_t i=0;i<N;i++){
		in[i] = fs[i].left;
	}
	
}
Color get_random_color(){
	Color color[10] = {WHITE,GREEN,BLUE,YELLOW,RED,PURPLE,GOLD,ORANGE,PINK,VIOLET};
	int random = GetRandomValue(1,  10);
	return color[random];
	
}
void fft(float in[], size_t stride, float complex out[],size_t n){
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

 	plug->music =LoadMusicStream("The_Weeknd_ft_Playboi_Carti_-_Timeless.mp3");
 	PlayMusicStream(plug->music);
 	
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
		float cellWidth = (float)w/N;

		// drawing one frame of a signal
		for(size_t i = 0;i<N;i++){
				float t = amp(out[i])/max_amp;
				DrawRectangle(i*cellWidth,h/2-h/2*t,1,h/2*t,get_random_color());
			
		}
		EndDrawing();
 	
 }
