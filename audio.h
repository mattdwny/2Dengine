#ifndef AUDIO_H
#define AUDIO_H

#define BATTLEFIELD 0
#define FINALDESTINATION 1

#define GO_ 0
#define ONE_ 1
#define TWO_ 2
#define THREE_ 3
#define GAME_ 4
#define TIME_ 5

void InitAudio();
void LoadSounds();
void PlayMusic(int i);
void PlaySound(int i);

#endif