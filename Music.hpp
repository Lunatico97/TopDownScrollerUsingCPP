
#pragma once
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <iostream>

class Music
{
	public :
		
		static Mix_Chunk *loadMusic(const char *filename) ;
		
		static void playMusic(Mix_Chunk *chunk, int volume) ;
		
		static void freeMusic(Mix_Chunk *chunk) ;
		
		static void quit() ;

} ;
