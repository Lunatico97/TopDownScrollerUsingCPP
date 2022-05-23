#pragma once

#include "Maths.hpp"
#include "Renderer.hpp"
#include "Entity.hpp"
#include "Music.hpp"

#define SCR_W 800
#define SCR_H 640

#define MAX_BUL 500 
#define MAX_MID 20
#define MAX_ARMS 3

#define MID_DP_CH 1.0
#define MID_DP_UNO 10.0
#define FIRE_RATE 5.0f 
#define PL_VEL 0.01f

class Game
{
	private :
		
		Renderer *renderer = nullptr ;
		SDL_Event event ;
		//Camera & Tiling Offset
		float cameraX, cameraY ;
		int tileW, tileH ;
		int visTilesX, visTilesY ;
		float offsetX, offsetY, tileoffsetX, tileoffsetY, dirX, dirY, diffX, diffY, dirBX, dirBY, diffBX, diffBY ;
		int tileID, layerID, mouseX, mouseY ;
		
		//Textures
		SDL_Texture *playerTex, *text, *tex[2], *bossTex, *temp, *item, *bullet, *midTex, *tiles, *laser, *arm_p ;
		SDL_Rect rect, desRect, srcRect, colliders[40][30] ;
		
		//Player Components	
		Entity *player = nullptr ;
		float playerX, playerY ;
		bool collided = false  ;
		bool left, right, down, top, shoot, leftShoot, rightShoot, downShoot, drawColliders ;
		float pointerAngle ;
		//Midget Components
		Entity *midgets[MAX_MID] = {nullptr} ;
		bool idleM[MAX_MID], walkM[MAX_MID], swirlM[MAX_MID], attackM[MAX_MID], hurtM[MAX_MID], deathM[MAX_MID] ;
		bool collidedM[MAX_MID] = {false} ;
		//Boss Components
		Entity *boss = nullptr ;
		bool blockB, idleB, walkB, laserB, attackB, shootB ;
		bool collidedB = false ;
		
		//Orb Balls Sprites
		std::vector<std::pair<int,int>> orb = {{192,64},{7*32,2*32},{8*32,2*32},{9*32,2*32}, {10*32,2*32}, {11*32,2*32}} ; //32,32
		//Arm Projectile Sprites
		std::vector<std::pair<int,int>> arm = {{60,30},{160,30},{270,30},{60,130},{160,130},{270,130}} ; //35, 15
		std::vector<std::pair<int,int>> lazer = {{30,110},{30,210},{30,310},{30,410},{30,510},{30,610},{30,710},
												 {30,810},{30,910},{30,1010},{30,1110},{30,1210},{30,1310},{30,1410}} ; //270, 40
												 
		//Player Animation Sources from sprite sheets
		std::vector<std::pair<int,int>> pyIdle = {{18,34},{80,34},{144,34},{210,34}} ; //24, 40
		std::vector<std::pair<int,int>> pyLeft = {{18,396},{80,396},{144,396},{210,396}} ; //24, 40
		std::vector<std::pair<int,int>> pyDown = {{18,324},{80,324},{144,324},{210,324}} ; //24, 40
		std::vector<std::pair<int,int>> pyUp = {{18,466},{80,466},{144,466},{210,466}} ; //24, 40
		std::vector<std::pair<int,int>> pyDownShoot = {{18,108},{80,108},{144,108}} ; //24, 40
		std::vector<std::pair<int,int>> pyLeftShoot = {{6,180},{50,180},{84,180}} ; //24, 40
		//Boss Animation Sources from sprite sheets
		std::vector<std::pair<int,int>> boIdle = {{20,120},{120,120},{220,120},{320,120},{420,120},{520,120},{620,120},{720,120}} ; //100, 100 - 60, 60
		std::vector<std::pair<int,int>> boBlock = {{20,300},{120,300},{220,300},{320,300},{420,300},{520,300},{620,300},{720,300}} ; //100, 100 - 60, 60
		std::vector<std::pair<int,int>> boLaser = {{20,520},{120,520},{220,520},{320,520},{420,520},{520,520},{620,520}} ; //100, 100 - 60, 60
		std::vector<std::pair<int,int>> boAttack = {{20,220},{120,220},{220,220},{320,220},{420,220},{520,220},{620,220},{720,220},{820,220}} ; //100, 100 - 60, 60
		std::vector<std::pair<int,int>> boWalk = {{20,620},{120,620},{220,620},{320,620},{420,620},{520,620},{620,620},{720,620},{820,620},{920,620}} ; //100, 100 - 60, 60
		//Midgets Animation Sources from sprite sheets
		std::vector<std::pair<int,int>> miIdle = {{20,10},{82,10},{148,10},{212,10},{276,10}} ; //26, 20
		std::vector<std::pair<int,int>> miWalk = {{20,39},{82,39},{148,39},{212,39},{276,39},{342,39},{404,39},{468,39}} ; //26, 20
		std::vector<std::pair<int,int>> miDeath = {{20,232},{82,232},{148,232},{212,232},{276,232},{342,232},{404,232}} ; //26, 20
		std::vector<std::pair<int,int>> miHurt = {{20,200},{82,200},{148,200},{212,200}} ; //26, 20
		std::vector<std::pair<int,int>> miSwirl = {{20,138},{82,138}} ; //26,20
		//Fonts
		TTF_Font *font ;
		SDL_Color white = {255,255,255,255} ;
		//Timers
		Uint64 currentTick ;
		Uint64 lastTick ;
		double deltaTime = 0 ;
		
		//Angle
		float getAngle(float x1, float y1, float x2, float y2){
			return atan2(y1 - y2, x2 - x1)*180/3.1415f ;
		}
		
		//Randomizer
		int randomize(int low, int high)
		{
			int ran ;
			ran = (rand()%((high+1)-low)+low) ;
			return ran ;
		}
		
		//Collision
		bool collisionAABB(SDL_Rect rect1, SDL_Rect rect2)
		{
			if(	(rect1.x <= rect2.x + rect2.w) &&
				(rect2.x <= rect1.x + rect1.w)&&
				(rect1.y <= rect2.y + rect2.h) &&
				(rect2.y <= rect1.y + rect1.h) )
			{
				return true ;
			}
			else
				return false ;
		}
		
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
		
		//Bullet Mechanics
		typedef struct
		{ float x, y, dx, dy;
		} Bullet ;
		//Create Bullets
		void addBullet(float x, float y, float dx, float dy, Bullet *bullets[])
		{
		  int found = -1;
		  for(int i = 0; i < MAX_BUL; i++)
		  {
		    if(bullets[i] == NULL)
		    {
		      found = i;
		      break;
		    }
		  }  
		  if(found >= 0)
		  {
		    int i = found;
		    bullets[i] = new Bullet ;
		    bullets[i]->x = x;
		    bullets[i]->y = y;
		    bullets[i]->dx = dx;    
		    bullets[i]->dy = dy;
		  }
		}
		//Delete bullets
		void removeBullet(int i, Bullet *bullets[])
		{
		  if(bullets[i])
		  {
		    delete bullets[i] ;
		    bullets[i] = NULL;
		  }
		}
		Bullet *bullets[MAX_BUL] = {NULL} ;
		Bullet *arms[MAX_ARMS] = {NULL} ;
		
		//Music
		Mix_Chunk *theme = nullptr ;
	
	public :
		
		Game() ;
		bool initSys_() ;
		void init_() ;
		void loadRsrc_() ;
		double timer() ;
		void render_() ;
		bool update_() ;
		void freeRsrc_() ;
		void freeSys_() ;		
	
} ;
