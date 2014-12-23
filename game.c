#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_thread.h"
#include "globals.h"
#include "graphics.h"
#include "glib.h"
#include "vector2.h"
#include "entity.h"
#include "fighter.h"
#include "controller.h"
#include "font.h"
#include "audio.h"
#include "network.h"
#include "levelEditor.h"

extern SDL_Surface* screen;
extern SDL_Surface* buffer; //pointer to the draw buffer
extern SDL_Rect Camera;
extern Entity* __entityList;
extern int netquit;

void InitAll();
void InitControllers(int map);
void InitFighters();
void InitNetwork(char* host);
void InitPlayer(char* str, float x, float y);
void InitPlayer(char* str, float x, float y, int c);
void CleanupAll();
void ProcessInput();
void CountDown(char* string, Uint32 color);

void TimeOut();
void WinGame(int i);

void LevelEditor(char* edit);
void TileLevel(char* level);

int quit = 0;
SDL_Thread* threads[3] = { NULL, NULL, NULL };

/**
 * this program must be run from the directory directly below images and src, not from within src
 * notice the default arguments for main.  SDL expects main to look like that, so don't change it
 */
int main(int argc, char** argv)
{
	SDL_Surface* temp;
	SDL_Surface* bg;
	int time = 9999;
	char timechar[2];
	char level[128] = "";
	char edit[128] = "";
	char host[32] = "";
	int param = 0;
	
	FILE* file = NULL;

	while(++param < argc && argv[param][0] == '-' && param + 1 < argc) //ultra meh
	{
		switch(argv[param][1])
		{
			case 'l':
				strncpy_s(level, "levels/", 8);
				strncpy(level+7, argv[++param], 121); // 127 + 1 + 1 == 121 + 8
				break;

			case 'e':
				strncpy_s(edit, "levels/", 8);
				strncpy(edit+7, argv[++param], 121);
				break;

			case 'h':
				strncpy_s(host, argv[++param], 128);
				break;

			default:
				CRASH("Illegal argument after hyphen");
				break;
		}
	}

	InitAll();

	temp = IMG_Load("images/fire_and_ice.jpg");

	if(temp != NULL) bg = SDL_DisplayFormat(temp); // ALWAYS check your pointers before you use them

	SDL_FreeSurface(temp);

	if(bg != NULL) SDL_BlitSurface(bg, NULL, buffer, NULL);

	if(strcmp(edit,"") != 0) LevelEditor(edit);
	else if(strcmp(level,"") != 0) TileLevel(level);

	if(strcmp(host,"") == 0)
	{
		InitControllers(3);
		//InitNetwork();
	}
	else if(tolower(*host) == 104 || tolower(*host) == 108 /*ascii h for host or l for localhost*/)
	{
		InitControllers(1);
		InitNetwork(NULL);
	}
	else
	{
		InitControllers(2);
		InitNetwork(host);
	}

	PlaySound(THREE_);
	FrameDelay(200);
	CountDown("3", Red_);
	PlaySound(TWO_);
	CountDown("2", Orange_);
	PlaySound(ONE_);
	CountDown("1", Yellow_);
	PlaySound(GO_);

	PlayMusic(BATTLEFIELD);
	//PlayMusic(FINALDESTINATION);
	while(!quit)
	{
		//BEGIN Draw Events
		ResetBuffer ();
		DrawEntityList();
 
		DrawTextCentered(itoa(time/100,timechar,10),screen, ResolutionX_/2 + 24, ResolutionY_/2 - 300, Green_, 0x1337babe, F_Large);
		if(time > 9500) DrawTextCentered("GO!",screen, ResolutionX_/2 + 60, ResolutionY_/2 - 200, Green_, 0x1337babe, F_Large);

		NextFrame();
		//END Draw Events


		//BEGIN Input
		ProcessInput();
		//END Input


		//BEGIN Action Events
		ThinkEntityList();
		PopulateQuadtrees();
		UpdateEntityList();
		//END Action Events

		time--;
		if(time < 0) TimeOut();
		else if(netquit >= 10) CRASH("Opponent Disconnected");
		else if(abs(__entityList[0].data.fighter.health) >= 100.0f) WinGame(1);
		else if(abs(__entityList[1].data.fighter.health) >= 100.0f) WinGame(0);
	}

	exit(0);   //technically this will end the program, but the compiler likes all functions that can return a value TO return a value

	return 0;
}

/**
 * Everything required for game clean-up, saving needs to go here
 */
void CleanUpAll()
{
	int i;
	for(i = 0; i < 3; i++) SDL_KillThread( threads[i] );

	CloseSprites();
	CloseEntityList();
}

/**
 * Everything required for game set-up
 */
void InitAll()
{
	InitGraphics();
	LoadFonts();
	InitAudio();
	LoadSounds();
	InitEntityList();
	InitFighters();
	//InitController();
	atexit(CleanUpAll);
}

void InitControllers(int map)
{
	OpenControllers(map);
	//threads[0] = SDL_CreateThread( InputThread,    "" );
}

void InitFighters()
{
	InitPlayer("images/hypersphere.png", 200, 736);
	InitPlayer("images/hypersphere2.png", 800, 736);
}

void InitNetwork(char* host)
{
	OpenNetwork(host);
	threads[1] = SDL_CreateThread( Send,    "" );
	threads[2] = SDL_CreateThread( Receive, "" );
}

void InitPlayer(char* str, float x, float y)
{
	InitPlayer(str, x, y, -1);
}

void InitPlayer(char* str, float x, float y, int c)
{
	Fighter* fighter = (Fighter*) GetEntity(FIGHTER);
	Transform* trans = &fighter->trans;

	fighter->sprite = LoadSprite(str, 64, 64, 192, c, c, c);
	fighter->frame = 0;
	trans->pos[0] = x;
	trans->pos[1] = y;
}

void CountDown(char* string, Uint32 color)
{
	ResetBuffer ();
	DrawEntityList();
 
	DrawTextCentered(string,screen, ResolutionX_/2 + 7, ResolutionY_/2 - 200, color, 0x1337babe, F_Large);

	NextFrame();
	FrameDelay(1000);
}

void TimeOut()
{
	PlaySound(TIME_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);

	exit(0);
}

void WinGame(int i)
{
	PlaySound(GAME_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);

	if(i == 0)
	{
		CountDown("Player 1 Wins!", LightBlue_);
		CountDown("Player 1 Wins!", LightBlue_);
		CountDown("Player 1 Wins!", LightBlue_);
		CountDown("Player 1 Wins!", LightBlue_);
		CountDown("Player 1 Wins!", LightBlue_);
	}
	else if(i == 1)
	{
		CountDown("Player 2 Wins!", LightOrange_);
		CountDown("Player 2 Wins!", LightOrange_);
		CountDown("Player 2 Wins!", LightOrange_);
		CountDown("Player 2 Wins!", LightOrange_);
		CountDown("Player 2 Wins!", LightOrange_);
	}

	exit(0);
}

void LevelEditor(char* edit)
{
	int grid[12][16];
	FILE* file = NULL;
	SDL_Surface* tile = NULL;

	fopen_s(&file, edit, "ab+"); //create the file if necessary

	fclose(file);

	fopen_s(&file, edit, "rb");

	for(int r = 0; r < 12; r++) for(int c = 0; c < 16; c++) grid[r][c] = 0;

	LoadLevel(file, (int*) grid);

	fclose(file);

	tile = IMG_Load("images/tile.png");
	
	InitMouse();

	while(!quit)
	{
		static Uint8* keys = NULL;
		static int mx = 500, my = 500;
		static int r, c;
		static SDL_Rect rectSrc, rectDst;
		
		rectDst.w = rectSrc.w = 64;
		rectDst.h = rectSrc.h = 64;
		rectDst.x = rectSrc.x = 0;
		rectDst.y = rectSrc.y = 0;

		SDL_PumpEvents();

		ResetBuffer ();
		if(SDL_GetMouseState(&mx,&my)) grid[(my/64)][(mx/64)] = !grid[(my/64)][(mx/64)];
		for(r = -1; ++r < 12; rectDst.y += 64)
		{
			rectDst.x = 0;
			for(c = -1; ++c < 16; rectDst.x += 64)
			{
				if(grid[r][c]) { SDL_BlitSurface(tile, &rectSrc, screen, &rectDst); }
			}
		}

		DrawMouse();
		NextFrame();

		keys = SDL_GetKeyState(NULL);
		if(keys[SDLK_ESCAPE]) quit = 1;

		SDL_Delay(6);
	}

	SDL_FreeSurface(tile);

	fopen_s(&file, edit, "wb");

	SaveLevel(file, (int*) grid);

	fclose(file);

	exit(0);
}

void TileLevel(char* level)
{
	SDL_Surface* tile;
	int grid[12][16];
	FILE* file = NULL;
	int r, c;
	SDL_Rect rectSrc, rectDst;

	rectDst.w = rectSrc.w = rectDst.h = rectSrc.h = 64;
	rectDst.x = rectSrc.x = rectDst.y = rectSrc.y = 0;

	tile = IMG_Load("images/tile.png");

	fopen_s(&file, level, "r");

	if(!file) CRASH("Level does not exist: ", level);

	for(int r = 0; r < 12; r++) for(int c = 0; c < 16; c++) grid[r][c] = 0;

	LoadLevel(file, (int*) grid);

	for(r = -1; ++r < 12; rectDst.y += 64)
	{
		rectDst.x = 0;
		for(c = -1; ++c < 16; rectDst.x += 64)
		{
			if(grid[r][c]) { SDL_BlitSurface(tile, &rectSrc, buffer, &rectDst); }
		}
	}

	fclose(file);
}