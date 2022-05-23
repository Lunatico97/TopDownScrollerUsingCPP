
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Maths.hpp"
#include "Entity.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>

class Renderer
{
	private :
		SDL_Window *window ;
		SDL_Renderer *ren ;
		int scrW, scrH ;
		
	public :
		Renderer(const char *title, int width, int height) ;
		SDL_Texture *loadTexture(const char *location) ;
		SDL_Texture* loadTextureCK(const char *path, Uint8 r, Uint8 g, Uint8 b) ;
		SDL_Texture* loadText(const char *text, TTF_Font *font, SDL_Color color) ;
		TTF_Font *loadFont(const char *location, int pt) ;
		void freeFont(TTF_Font *font) ;
		void freeTex(SDL_Texture *t) ;
		void cleanRenderer() ;
		void clear() ;
		void render(Entity& entity) ;
		void render(SDL_Texture *t, SDL_Rect des) ;
		void render(int x, int y, SDL_Texture *tex) ;
		void render(SDL_Texture *t, SDL_Rect src, SDL_Rect des) ;
		void renderEx(SDL_Texture *t, SDL_Rect src, SDL_Rect des, float angle, SDL_RendererFlip flipMode, SDL_Point point) ;
		void renderRect(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, bool fill) ;
		void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) ;
		void display() ;
} ;
