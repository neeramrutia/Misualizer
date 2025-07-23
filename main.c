#include <raylib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])
#define N 256

float in[N];
float pi;
float complex out[N];
float max_amp;
typedef struct{
	float left;
	float right;
} Frame;


float amp(float complex z){
	float a = fabsf(crealf(z));
	float b = fabsf(cimagf(z));
	if(a<b) return b;
	return a; 	
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
		float complex v = cexp(-2*I*pi*t)*out[k+n/2];
		float complex e = out[k];
		out[k] = e+v;
		out[k+n/2] = e-v;
	}
}

void callback(void *bufferdata, unsigned int frames){
	if(frames < N) return;

	Frame *fs = bufferdata;
	for(size_t i=0;i<N;i++){
		in[i] = fs[i].left;
	}
	fft(in,1,out,N); 
	max_amp = 0.0f;
	for(size_t i=0;i<N;i++){
		float a = amp(out[i]); 
		if(max_amp < a) max_amp = a;
	}
}

int main(void)

{
	pi = atan2f(1,1)*4;
	InitWindow(800,600,"Musializer");
	SetTargetFPS(60);

	InitAudioDevice();
	Music music =LoadMusicStream("The_Weeknd_ft_Playboi_Carti_-_Timeless.mp3");
	PlayMusicStream(music);

	AttachAudioStreamProcessor(music.stream, callback);

	while(!WindowShouldClose()){
		UpdateMusicStream(music);

		if(IsKeyPressed(KEY_SPACE)){
			if(IsMusicStreamPlaying(music)){
				PauseMusicStream(music);
			}else{
				ResumeMusicStream(music);
			}
		}

		int w = GetRenderWidth();
		int h = GetRenderHeight();

		BeginDrawing();
		ClearBackground(BLACK);
		float cellWidth = (float)w/N;

		// drawing one frame of a signal
		for(size_t i = 0;i<N;i++){
				float t = amp(out[i])/max_amp;
				DrawRectangle(i*cellWidth,h/2-h/2*t,1,h/2*t,RED);
			
		}
		EndDrawing();
	}
	
    return 0;
}
