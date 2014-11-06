#include "SDL.h"
#include "SDL_mixer.h"
#include "globals.h"
#include "audio.h"

Mix_Music* music[2];
Mix_Chunk* sounds[6];

void InitAudio()
{   
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 11025, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) CRASH("Audio couldn't Load");
    
    //Set the window caption
    SDL_WM_SetCaption( "Monitor Music", NULL );
}

void LoadSounds()
{
	//Load the music
    music[0] = Mix_LoadMUS( "audio/music/battlefield.wav" );
	music[1] = Mix_LoadMUS( "audio/music/finaldestination.wav" );
    
    //If there was a problem loading the music
    if( !music[0] || !music[1] )
    {
		CRASH("Music failed to load");
    }

    //Load the sound effects
	sounds[0] = Mix_LoadWAV( "audio/sounds/GO.wav" );
	sounds[1] = Mix_LoadWAV( "audio/sounds/one.wav" );
	sounds[2] = Mix_LoadWAV( "audio/sounds/two.wav" );
	sounds[3] = Mix_LoadWAV( "audio/sounds/three.wav" );
	sounds[4] = Mix_LoadWAV( "audio/sounds/game.wav" );
	sounds[5] = Mix_LoadWAV( "audio/sounds/time.wav" );
}

void PlayMusic(int i)
{
	Mix_PlayMusic( music[i], -1 );
}

void PlaySound(int i)
{
	Mix_PlayChannel( -1, sounds[i], 0 );
}