
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Maths.hpp"

class Entity
{
	private :
	
		Vector2f position, velocity, render_position ;
		Vector2f collider_ratio = Vector2f(1,1) ;
		int width, height ;
		float angle = 0 ;
		Vector2f scale = Vector2f(1,1) ;
		SDL_Rect current_frame ;
		SDL_Texture *texture ;
		SDL_RendererFlip flipMode = SDL_FLIP_NONE ;
		
		bool checkCollisionAABB( SDL_Rect a, SDL_Rect b )
		{
		    //The sides of the rectangles
		    int leftA, leftB;
		    int rightA, rightB;
		    int topA, topB;
		    int bottomA, bottomB;
		    //Calculate the sides of rect A
		    leftA = a.x;
		    rightA = a.x + a.w;
		    topA = a.y;
		    bottomA = a.y + a.h;
		    //Calculate the sides of rect B
		    leftB = b.x;
		    rightB = b.x + b.w;
		    topB = b.y;
		    bottomB = b.y + b.h ;
		    //If any of the sides from A are outside of B
		    if( bottomA <= topB ) return false ;
		    if( topA >= bottomB ) return false ;
		    if( rightA <= leftB ) return false ;
		    if( leftA >= rightB ) return false ;
		    //If none of the sides from A are outside B
		    return true;
		}
		
	public :
		
		SDL_Rect collider ;
		float health = 100.0f ;
		float dummyVar ;
		
		Entity(Vector2f pos, SDL_Texture *tex) ;
		Entity(Vector2f spr_pos, Vector2f spr_dim, Vector2f pos, SDL_Texture *tex) ;	
		//Getters
		Vector2f& getPosition() { return position ;}
		Vector2f& getVelocity() { return velocity ;}
		Vector2f& getRenderPosition() { return render_position ;}
		float getAngle(){ return angle ;}
		int getWidth(){ return width ;}
		int getHeight(){ return height ;}
		Vector2f getScale(){ return scale ;}
		SDL_Texture *getTexture(){ return texture ;} 
		SDL_Rect getCurrentFrame(){ return current_frame ;} 
		SDL_RendererFlip getFlipMode(){ return flipMode ;}
		//Setters
		void setPosition(float x, float y) ;
		void setVelocity(float x, float y) ;
		void setDimensions(int w, int h) ;
		void setScale(float wm, float hm) ;
		void setAngle(float theta) ;
		void setFlipMode(SDL_RendererFlip mode) ;
		void setColliderRatio(float x, float y) ;
		void setRenderPosition(float x, float y) ;
		void setCurrentFrame(int x, int y) ;
		//Updates
		void update(double deltaTime, bool collisionCheck) ;  
		void updateRender(float offsetX, float offsetY, int relW, int relH) ;		
	
} ;
