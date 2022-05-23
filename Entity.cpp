
#include "Entity.hpp"

Entity::Entity(Vector2f pos, SDL_Texture *tex) : position(pos), texture(tex)
{
	current_frame = {0,0,32,32} ;
	SDL_QueryTexture(texture, NULL, NULL, &current_frame.w, &current_frame.h) ;
}

Entity::Entity(Vector2f spr_pos, Vector2f spr_dim, Vector2f pos, SDL_Texture *tex) : position(pos), texture(tex)
{
	current_frame = {spr_pos.x, spr_pos.y, spr_dim.x, spr_dim.y} ;
	velocity = Vector2f(0,0) ;
	position.x = pos.x ;
	position.y = pos.y ;
}

void Entity::setPosition(float x, float y)
{
	position.x = x ;
	position.y = y ;
}

void Entity::setRenderPosition(float x, float y)
{
	render_position.x = x ;
	render_position.y = y ;
}

void Entity::setCurrentFrame(int x, int y)
{
	current_frame.x = x ;
	current_frame.y = y ;
}

void Entity::setDimensions(int w, int h){
	width = w ;
	height = h ;
} 

void Entity::setScale(float wm, float hm)
{
	scale.x = wm ;
	scale.y = hm ;
}
void Entity::setColliderRatio(float cx, float cy)
{
	collider_ratio.x = cx ;
	collider_ratio.y = cy ;
}

void Entity::setAngle(float theta)
{
	angle = theta ;
}

void Entity::setFlipMode(SDL_RendererFlip mode)
{
	flipMode = mode ;
}

void Entity::setVelocity(float x, float y){
	velocity.x = x ;
	velocity.y = y ;
}

void Entity::update(double deltaTime, bool collisionCheck){
	//If collision, resolve it ! (I mean - push it !)
	int bop = 3 ; //Push value - (-_-)
		position.x += velocity.x*deltaTime ;
		if(collisionCheck){
			position.x -= velocity.x*bop*deltaTime ;
			velocity.x = 0 ;
		}
		position.y += velocity.y*deltaTime ;
		if(collisionCheck){
			position.y -= velocity.y*bop*deltaTime ;
			velocity.y = 0 ;
		}
}

void Entity::updateRender(float offsetX, float offsetY, int relW, int relH)
{
	render_position.x = (position.x-offsetX)*relW ;
	render_position.y = (position.y-offsetY)*relH ;
	//top left standard - collider = {render_position.x - (scale.x*width - width)/2, render_position.y - (scale.y*height - height)/2, width*scale.x, height*scale.y} ;
	collider = {render_position.x - (scale.x*(width*collider_ratio.x))/2, 
	            render_position.y - (scale.y*(height*collider_ratio.y))/2,
				width*collider_ratio.x*scale.x, height*collider_ratio.y*scale.y} ;
}


