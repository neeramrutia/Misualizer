		

#include <raylib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])
// global frame count = filled array of global frames
// capacity  = size of global frame  , capacit always > global frame count 
// frames = size of input chunk in callback for processing
// bufferdata = main pointer which points to buffer of raylib --> dont modify bufferdata
typedef struct{
	float left;
	float right;
} Frame;
Frame global_frames[4800] = {0};
size_t global_frames_count = 0;
void callback(void *bufferdata, unsigned int frames){
	size_t capacity = ARRAY_LEN(global_frames);
	if(frames <= capacity - global_frames_count){
		memcpy(global_frames + global_frames_count, bufferdata, sizeof(Frame)*frames);
		global_frames_count += frames;
	}else if(frames <= capacity){
		memmove(global_frames, global_frames+frames,sizeof(Frame)*(capacity-frames));
		memcpy(global_frames + (capacity-frames), bufferdata, sizeof(Frame)*frames);
	}else{
		memcpy(global_frames,bufferdata,sizeof(Frame)*capacity);
		global_frames_count = capacity;
	}

//	printf("frames : %u" ,frames );


}
int main(void)

{
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
		float cellWidth = (float)w/global_frames_count;

		// drawing one frame of a signal
		for(size_t i = 0;i<global_frames_count;i++){
			float t = global_frames[i].left;
			
			//	printf(" %d " , sample);
			if(t>0){
				DrawRectangle(i*cellWidth,h/2-h/2*t,1,h/2*t,RED);		
			}else{
				DrawRectangle(i*cellWidth,h/2,1,h/2*t,RED);
			}
		
			
		}
		EndDrawing();
	}
	
    return 0;
}
