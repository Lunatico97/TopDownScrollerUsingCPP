
#include "Game.hpp"
#include "Map.hpp"

Game::Game(){}

bool Game::initSys_(){
	renderer = new Renderer("Serious Sam", SCR_W, SCR_H)  ;
	return true ;
}

void Game::loadRsrc_(){
	
	//Unpacking resources
	playerTex = renderer->loadTexture("../rsrc/gfx/pyro.png") ;
	bossTex = renderer->loadTexture("../rsrc/gfx/golem.png") ;
	midTex = renderer->loadTextureCK("../rsrc/gfx/dwarf.png",255,0,0) ;
	bullet = renderer->loadTexture("../rsrc/gfx/orbs.png") ;
	tiles = renderer->loadTexture("../rsrc/gfx/tiles.png") ;
	arm_p = renderer->loadTexture("../rsrc/gfx/golem_attack/arm.png") ;
	font = renderer->loadFont("../rsrc/fonts/font.ttf", 20) ;	
	text = renderer->loadText("IDLE", font, white) ;
	//Tiles
	tex[0] = renderer->loadTexture("../rsrc/gfx/Tile_12.png") ;
	tex[1] = renderer->loadTexture("../rsrc/gfx/Tile_31.png") ;
	//Game texture initializers
	item = renderer->loadTexture("../rsrc/gfx/TX Props.png") ;
	//Music
	theme = Music::loadMusic("../rsrc/sfx/Corridor Of Death.mp3") ;
	
}

double Game::timer(){

	lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	std::cout << SDL_GetPerformanceCounter() << std::endl ;
	//double dt = (double)((currentTick - lastTick)*1000 / (double)2241001 );
	double dt = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );
	return dt ;
	
}

void Game::init_(){
	
	//Scene Initialization
	cameraX = 0.0f;  
	cameraY = 0.0f;
	tileW = 32 ;
	tileH = 32 ;
	visTilesX = SCR_W/tileW ;
	visTilesY = SCR_H/tileH ;
	drawColliders = false ;
	
	//Player Initialization
	Vector2f spr = {(float)pyIdle[0].first, (float)pyIdle[0].second} ;
	player = new Entity(spr, Vector2f(24,40), Vector2f(0,0), playerTex) ;
	player->setScale(1,1) ;
	player->setPosition(10,60) ;
	player->setDimensions(24, 40) ;
	player->setColliderRatio(1,1) ;
	
	//Midgets Initialization
	Vector2f sprM = {(float)miIdle[0].first, (float)miIdle[0].second} ;
	srand(time(NULL)) ;
	for(int i=0; i<MAX_MID; i++){
		midgets[i] = new Entity(sprM, Vector2f(26,20), Vector2f(0,0), midTex) ;
		midgets[i]->setScale(1,1) ;
		midgets[i]->setPosition(randomize(10,70),randomize(10,70)) ;
		midgets[i]->setDimensions(48, 36) ;
		midgets[i]->setColliderRatio(0.6,0.6) ;
	}
	//Boss Initialization
	Vector2f sprB = {(float)boIdle[0].first, (float)boIdle[0].second} ;
	boss = new Entity(sprB, Vector2f(60,70), Vector2f(0,0), bossTex) ;
	boss->setScale(1,1) ;
	boss->setPosition(40,15) ;
	boss->setDimensions(100, 100) ;
	boss->setColliderRatio(0.6,0.6) ;
	
	//Timer Initialization
	currentTick = SDL_GetPerformanceCounter() ;
	lastTick = 0 ;
}

bool Game::update_(){
	//Generate delta time
	deltaTime = timer() ;
	renderer->freeTex(text) ;
	//Get our controls and events
	while (SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT) return false ;
		if(event.type == SDL_MOUSEBUTTONDOWN) shoot = true ;
		if(event.type == SDL_MOUSEBUTTONUP) shoot = false ;
		if(event.type == SDL_KEYDOWN && event.key.repeat == 0){
						switch(event.key.keysym.sym){
							case SDLK_ESCAPE : 
								return false ;
								break ;
							case SDLK_d :
								right = true ;
								player->setVelocity(PL_VEL,0) ;
								break ;
							case SDLK_a :
								left = true ;
								player->setVelocity(-PL_VEL,0) ;
								break ;
							case SDLK_s :
								down = true ;
								player->setVelocity(0,PL_VEL) ;
								break ;	
							case SDLK_w :
								top = true ;
								player->setVelocity(0,-PL_VEL) ;
								break ;	
							case SDLK_c :
								drawColliders = !drawColliders ;
								break ;
							default:
								break ;
				}
		}

		if(event.type == SDL_KEYUP){
			top = down = left = right = false ;
					switch(event.key.keysym.sym){

						case SDLK_d :
							player->setVelocity(0,0) ;
							
							break ;
						case SDLK_a :
							player->setVelocity(0,0) ;
					
							break ;
						case SDLK_w :
							player->setVelocity(0,0) ;
						
							break ;
						case SDLK_s :
							player->setVelocity(0,0) ;
						
							break ;
					
						default:
							break ;
					}
			}
	}
	
	//Collision of entities with tilemap
	for(int i=0; i<visTilesX; i++){
		for(int j=0; j<visTilesY; j++){
			if(collisionAABB(colliders[i][j], player->collider)){
				if(getTile(i+offsetX,j+offsetY) == 1 || getLayer(i+offsetX,j+offsetY) == 41 ||  getLayer(i+offsetX,j+offsetY) == 42){
					collided = true ;
				}
			}	
			if(collisionAABB(colliders[i][j], boss->collider)){
				if(getTile(i+offsetX,j+offsetY) == 1 || getLayer(i+offsetX,j+offsetY) == 41 ||  getLayer(i+offsetX,j+offsetY) == 42){
					collidedB = true ;
				}
			}	
			for(int k=0; k<MAX_MID; k++){
				if(collisionAABB(colliders[i][j], midgets[k]->collider) && !deathM[k]){
					if(getTile(i+offsetX,j+offsetY) == 1 || getLayer(i+offsetX,j+offsetY) == 41 ||  getLayer(i+offsetX,j+offsetY) == 42){
						collidedM[k] = true ;
					}
				}	
			}
		}
	}	
	
	//Collision between Entities
	if(collisionAABB(player->collider, boss->collider)) collided = true ;
	for(int i=0; i<MAX_MID; i++) if(collisionAABB(player->collider, midgets[i]->collider) && !deathM[i] )
		 collided = true ;
	for(int i=0; i<MAX_MID; i++) if(collisionAABB(midgets[i]->collider, midgets[i==MAX_MID-1 ? 0 : i+1]->collider) && !deathM[i] && !deathM[i==MAX_MID-1 ? 0 : i+1])
		 collidedM[i] = true ;
	for(int i=0; i<MAX_MID; i++) if(!deathM[i]) 
		idleM[i] = true ;
	idleB = true ;
	
	//Update movement based on delta time between frames to smooth movement rather than delay frames and avoid getting your head on the walls	
	
	//Update Movements and Collisions
	player->update(deltaTime, collided) ;
	boss->update(deltaTime, collidedB) ;
	for(int i=0; i<MAX_MID; i++)
		if(!deathM[i])
			midgets[i]->update(deltaTime, collidedM[i]) ;
			
	//std::cout << deltaTime << std::endl ;
	
	//We can't restrict movement just because it collided once, can we ?
	collided = false ; 
	collidedB = false ;	
	for(int i=0; i<MAX_MID; i++)
		if(!deathM[i])
			collidedM[i] = false ;	
			
	playerX = player->getPosition().x ;
	playerY = player->getPosition().y ;
	
	//Position Log
	//std::cout << playerX << "," << playerY << std::endl ;
	
	//Player Boundary Capping
			
	if(playerX  <= 0 ) playerX = 0 ;
	if(playerY <= 0 ) playerY = 0 ;
	if(playerX >= LVL_W-1) playerX = LVL_W-1 ;
	if(playerY >= LVL_H-1) playerY = LVL_H-1 ;	
	player->setPosition(playerX, playerY) ;		
	
	//Aim Mechanics
	SDL_GetMouseState(&mouseX, &mouseY) ;
	float pointerAngle = getAngle(player->getRenderPosition().x, player->getRenderPosition().y, mouseX, mouseY) ;
	//std::cout << pointerAngle << std::endl ;
	diffX = (mouseX - player->getRenderPosition().x) ;
	diffY = (mouseY - player->getRenderPosition().y) ;
	if(mouseX != player->getRenderPosition().x) dirX = diffX/fabs(mouseX - player->getRenderPosition().x) ;
	if(mouseY != player->getRenderPosition().y) dirY = diffY/fabs(-player->getRenderPosition().y + mouseY) ;
	
	//Bullet Mechanics
	if(shoot){
		//if(dirY < 0) downShoot = true ;
		//else if(dirX > 0) leftShoot = true ;
		//else if(dirX < 0) rightShoot = true ;
		dirX *= fabs(cos((pointerAngle)*3.1415f/180))*FIRE_RATE ;
		dirY *= fabs(sin((pointerAngle)*3.1415f/180))*FIRE_RATE ;
		addBullet(player->getRenderPosition().x + 10*dirX/3, player->getRenderPosition().y + 10*dirY/3, dirX, dirY, bullets)  ;
		shoot = false ;
	}
	
	//Boss Mechanics (Tracking)
	float angle = getAngle(boss->getPosition().x, boss->getPosition().y, player->getPosition().x, player->getPosition().y) ;
	diffBX = (player->getPosition().x - boss->getPosition().x) ;
	diffBY = (player->getPosition().y - boss->getPosition().y) ;
	if(boss->getPosition().x != player->getPosition().x) dirBX = diffBX/fabs(player->getPosition().x - boss->getPosition().x) ;
	if(boss->getPosition().y != player->getPosition().y) dirBY = diffBY/fabs(-boss->getPosition().y + player->getPosition().y) ;
	dirBX *= fabs(cos((angle)*3.1415f/180)) ;
	dirBY *= fabs(sin((angle)*3.1415f/180)) ;
	//boss->getPosition().logs() ;
	if(boss->getRenderPosition().getDistance(player->getRenderPosition()) > 200 && boss->getRenderPosition().getDistance(player->getRenderPosition()) < 400)
	{
		boss->setVelocity(dirBX*0.001f, dirBY*0.001f) ;
		walkB = true ;
		laserB = false ;
	}
	else if(boss->getRenderPosition().getDistance(player->getRenderPosition()) < 100 )
	{
		boss->setVelocity(0,0) ;
		blockB = true ;
		laserB = false ;
		walkB = false ;
		idleB = false ;
		attackB = false ;
	}
	else if(boss->getRenderPosition().getDistance(player->getRenderPosition()) > 400 )
	{
		attackB = true ;
		walkB = false ;
		blockB = false ;
		idleB = false ;
	}
	else{
		boss->setVelocity(0,0) ;
		idleB = true ;
		walkB = false ;
		blockB = false ;
		attackB = false ;
	}
	
	//Boss Aim
	if(shootB){
		//if(dirY < 0) downShoot = true ;
		//else if(dirX > 0) leftShoot = true ;
		//else if(dirX < 0) rightShoot = true ;
		dirBX *= fabs(cos((angle)*3.1415f/180))*3 ;
		dirBY *= fabs(sin((angle)*3.1415f/180))*3 ;
		addBullet(boss->getRenderPosition().x + 30*dirBX/3, boss->getRenderPosition().y + 30*dirBY/3, dirBX, dirBY, arms)  ;
		shootB = false ;
	}
	
	//Midgets Tracking And Attack
	for(int i=0; i<MAX_MID; i++){
		if(!deathM[i]){
		float angle = getAngle(midgets[i]->getPosition().x, midgets[i]->getPosition().y, player->getPosition().x, player->getPosition().y) ;
		diffBX = (player->getPosition().x - midgets[i]->getPosition().x) ;
		diffBY = (player->getPosition().y - midgets[i]->getPosition().y) ;
		if(midgets[i]->getPosition().x != player->getPosition().x) dirBX = diffBX/fabs(player->getPosition().x - midgets[i]->getPosition().x) ;
		if(midgets[i]->getPosition().y != player->getPosition().y) dirBY = diffBY/fabs(-midgets[i]->getPosition().y + player->getPosition().y) ;
		dirBX *= fabs(cos((angle)*3.1415f/180)) ;
		dirBY *= fabs(sin((angle)*3.1415f/180)) ;
		//midgets[i]->getPosition().logs() ;
		if(midgets[i]->getRenderPosition().getDistance(player->getRenderPosition()) < 300){
			if(midgets[i]->getRenderPosition().getDistance(player->getRenderPosition()) > 45)
			{	
				midgets[i]->setVelocity(dirBX*0.005f, dirBY*0.005f) ;
				walkM[i] = true ;
			}
			else{
				midgets[i]->setVelocity(0,0) ;
				swirlM[i] = true ;
			}
		}
		else{
			midgets[i]->setVelocity(0,0) ;
			idleM[i] = true ;
		}
	}
	}
	
	//Camera Movement and Tile Offset		
			
	cameraX = playerX ; 
	cameraY = playerY ;
			
	offsetX = cameraX - (float)visTilesX/2.0f ;
	offsetY = cameraY - (float)visTilesY/2.0f ;
			
	if(offsetX < 0) offsetX = 0 ;
	if(offsetY < 0) offsetY = 0 ;
	if(offsetX > LVL_W - visTilesX) offsetX = LVL_W - visTilesX ;
	if(offsetY > LVL_H - visTilesY) offsetY = LVL_H - visTilesY ;
			
	tileoffsetX = (offsetX - int(offsetX))*tileW ;
	tileoffsetY = (offsetY - int(offsetY))*tileH ;
	
	player->updateRender(offsetX, offsetY, tileW, tileH) ;
	boss->updateRender(offsetX, offsetY, tileW, tileH) ;
	for(int i=0; i<MAX_MID; i++)
		midgets[i]->updateRender(offsetX, offsetY, tileW, tileH) ;
	
	return true ;
}

void Game::render_(){
	renderer->setColor(0,0,0, 255) ;
	renderer->clear() ;
	//Render elements -----------------------------------------------
	//Tiling with respect to sidescrolling movement along with camera
	for(int x=-1; x<visTilesX+1; x++){
		for(int y=-1; y<visTilesY+1; y++){
			tileID = getTile(x+offsetX, y+offsetY) ;
			rect = {x*tileW - tileoffsetX, y*tileH - tileoffsetY, tileW, tileH} ;
			//Get Colliders
			if(x>-1 && y>-1)colliders[x][y] = {x*tileW - tileoffsetX, y*tileH - tileoffsetY, tileW, tileH} ;
			//Tiling
			switch(tileID){
					case 0 :
						temp = tex[0] ;
						break ;	
						
					case 1 :
						temp = tex[1] ;							
					    break ;						
					
					default:
						break ;
			}
			renderer->render(temp, rect) ;
		}
	}
			
	// !!! WARNING : Manual Data - DON'T CHANGE IT UNLESS YOU CAN SET IT AGAIN !!!		
	for(int x=-1; x<visTilesX+1; x++){
		for(int y=-1; y<visTilesY+1; y++){
			layerID = getLayer(x+offsetX, y+offsetY) ;
				desRect = {x*tileW - tileoffsetX, y*tileH - tileoffsetY, tileW, tileH} ;
				switch(layerID){
					case 01 :
						srcRect = {24,8,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;
					case 11 :
						srcRect = {8,0,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;
					case 12 :
						srcRect = {8,8,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;
					case 13 :
						srcRect = {8,16,8,8} ;
						renderer->render(  tiles, srcRect, desRect) ;
						break ;
					case 14 :
						srcRect = {8,24,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;
					case 15 :
						srcRect = {24,24,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;	
					case 16 :
						srcRect = {8,32,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;
					case 21 :
						srcRect = {0,40,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;	
					case 22 :
						srcRect = {24,32,8,8} ;
						renderer->render(  tiles , srcRect, desRect) ;
						break ;
					case 31 :
						srcRect = {350,265,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 32 :
						srcRect = {350+34,265,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 33 :
						srcRect = {350+34*2,265,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 34 :
						srcRect = {350,265+27,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 35 :
						srcRect = {350+34,265+27,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 36 :
						srcRect = {350+2*34,265+27,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 37 :
						srcRect = {350,265+2*27,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 38 :
						srcRect = {350+34,265+2*27,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 39 :
						srcRect = {350+2*34,265+2*27,34,27} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;
					case 41 :
						srcRect = {445,20,40,35} ;
						renderer->render(  item , srcRect, desRect) ;						
						break ;	
					case 42 :
						srcRect = {445,20+35,40,35} ;
						renderer->render(  item , srcRect, desRect) ;
						break ;	
					default:
						break ;
				}	
		}
	}
	
	//Player Animation Bools
	renderer->freeTex(text) ;
	if(top) text = renderer->loadText("UP", font, white) ;
	if(down) text = renderer->loadText("DOWN", font, white) ;
	if(left) text = renderer->loadText("LEFT", font, white) ;
	if(right) text = renderer->loadText("RIGHT", font, white) ;
	system("@cls||clear") ;
	int time ;
			if(left)
			{	
				time = (SDL_GetTicks()/100)%pyLeft.size();
				player->setCurrentFrame(pyLeft[time].first, pyLeft[time].second) ;
				player->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(right){
				time = (SDL_GetTicks()/100)%pyLeft.size();
				player->setCurrentFrame(pyLeft[time].first, pyLeft[time].second) ;
				player->setFlipMode(SDL_FLIP_HORIZONTAL) ;
			}
			if(down){	
				time = (SDL_GetTicks()/100)%pyDown.size();
				player->setCurrentFrame(pyDown[time].first, pyDown[time].second) ;
				player->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(top){
				time = (SDL_GetTicks()/100)%pyUp.size();
				player->setCurrentFrame(pyUp[time].first, pyUp[time].second) ;
				player->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(downShoot)
			{	
				time = (SDL_GetTicks()/100)%pyDownShoot.size();
				player->setCurrentFrame(pyDownShoot[time].first, pyDownShoot[time].second) ;
				player->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(leftShoot)
			{	
				time = (SDL_GetTicks()/100)%pyLeftShoot.size();
				player->setCurrentFrame(pyLeftShoot[time].first, pyLeftShoot[time].second) ;
				player->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(rightShoot)
			{	
				time = (SDL_GetTicks()/100)%pyLeftShoot.size();
				player->setCurrentFrame(pyLeftShoot[time].first, pyLeftShoot[time].second) ;
				player->setFlipMode(SDL_FLIP_HORIZONTAL) ;
			}
			if(!shoot)
				downShoot = leftShoot = rightShoot = false ;
	
	//Midget Animation Bools
	int timeM ;
	for(int i=0; i<MAX_MID; i++){
			if(idleM[i])
			{	
				timeM = (SDL_GetTicks()/100)%miIdle.size();
				midgets[i]->setCurrentFrame(miIdle[timeM].first, miIdle[timeM].second) ;
				midgets[i]->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(walkM[i])
			{	
				idleM[i] = false ;
				timeM = (SDL_GetTicks()/100)%miWalk.size();
				midgets[i]->setCurrentFrame(miWalk[timeM].first, miWalk[timeM].second) ;
				if(right) midgets[i]->setFlipMode(SDL_FLIP_NONE) ;
				else midgets[i]->setFlipMode(SDL_FLIP_HORIZONTAL) ;
			}	
			if(hurtM[i])
			{	
				idleM[i] = false ;
				midgets[i]->health -= MID_DP_UNO ;
				timeM = (SDL_GetTicks()/100)%miHurt.size();
				midgets[i]->setCurrentFrame(miHurt[timeM].first, miHurt[timeM].second) ;
				if(right) midgets[i]->setFlipMode(SDL_FLIP_NONE) ;
				else midgets[i]->setFlipMode(SDL_FLIP_HORIZONTAL) ;
			}	
			if(swirlM[i])
			{	
				idleM[i] = false ;
				timeM = (SDL_GetTicks()/100)%miSwirl.size();
				midgets[i]->setCurrentFrame(miSwirl[timeM].first, miSwirl[timeM].second) ;
			}	
			
			if(midgets[i]->health < 0)
				deathM[i] = true ;	
				
			if(deathM[i])
			{	
				idleM[i] = false ;
				midgets[i]->setVelocity(0,0) ;
				if(timeM < miDeath.size()){
					timeM = (SDL_GetTicks()/100)%miDeath.size();
					midgets[i]->setCurrentFrame(miDeath[timeM].first, miDeath[timeM].second) ;
				} 
				midgets[i]->setCurrentFrame(miDeath[miDeath.size()-1].first, miDeath[miDeath.size()-1].second) ;
			}	
	}
	for(int i=0; i<MAX_MID; i++){
		walkM[i] = false ;
		swirlM[i] = false ;
		hurtM[i] = false ;
	}
	
	//Boss Animation Bools
	int timeB ;
			if(idleB)
			{	
				timeB = (SDL_GetTicks()/100)%boIdle.size();
				boss->setCurrentFrame(boIdle[timeB].first, boIdle[timeB].second) ;
				boss->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(blockB)
			{	
				timeB = (SDL_GetTicks()/100)%boBlock.size();
				if(timeM < boBlock.size()){
					timeM = (SDL_GetTicks()/100)%boBlock.size();
					boss->setCurrentFrame(boBlock[timeB].first, boBlock[timeB].second) ;
				} 
				boss->setCurrentFrame(boBlock[boBlock.size()-1].first, boBlock[boBlock.size()-1].second) ;
				boss->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(walkB)
			{	
				idleB = false ;
				timeB = (SDL_GetTicks()/100)%boWalk.size();
				boss->setCurrentFrame(boWalk[timeB].first, boWalk[timeB].second) ;
				boss->setFlipMode(SDL_FLIP_NONE) ;
			}
			if(attackB)
			{	
				shootB = true ;
				idleB = false ;
				timeB = (SDL_GetTicks()/100)%boAttack.size();
				boss->setCurrentFrame(boAttack[timeB].first, boAttack[timeB].second) ;
				if(right) boss->setFlipMode(SDL_FLIP_NONE) ;
				else boss->setFlipMode(SDL_FLIP_HORIZONTAL) ;
			}
			
			if(laserB)
			{	
				idleB = false ;
				timeB = (SDL_GetTicks()/100)%boLaser.size();
				boss->setCurrentFrame(boLaser[timeB].first, boLaser[timeB].second) ;
			}

	//Entity Rendering
	for(int i=0; i<MAX_MID; i++)
		renderer->render(*midgets[i]) ;
	renderer->render(*player) ;
	renderer->render(20,20,text) ;
	renderer->render(*boss) ;
	
	//Bullet Rendering
	for(int i = 0; i < MAX_BUL; i++) if(bullets[i])
  	{
    	SDL_Rect rect = { bullets[i]->x, bullets[i]->y, 16, 16 };
		int timeBul ;
		timeBul = (SDL_GetTicks()%100)%orb.size() ;  
    	SDL_Rect src = {orb[timeBul].first, orb[timeBul].second, 32, 32} ;
    	renderer->render(bullet, src, rect) ;
  	}
  	for(int i = 0; i < MAX_ARMS; i++) if(arms[i])
  	{
    	SDL_Rect rect = { arms[i]->x, arms[i]->y, 30, 20 };
		int timeBul ;
		timeBul = (SDL_GetTicks()%100)%arm.size() ;  
    	SDL_Rect src = {arm[timeBul].first, arm[timeBul].second, 35, 15} ;
    	renderer->render(arm_p, src, rect) ;
  	}
  	
  	//Bullet Disposal
	for(int i = 0; i < MAX_BUL; i++) if(bullets[i])
	{	
	  	bullets[i]->x += bullets[i]->dx ;
	  	bullets[i]->y += bullets[i]->dy ;
		SDL_Rect rect = { bullets[i]->x, bullets[i]->y, 16, 16 }; 	  
		SDL_Rect temp_tile = {bullets[i]->x, bullets[i]->y, tileW, tileH} ;	
		if(getTile((temp_tile.x+tileoffsetX)/tileW + offsetX, (temp_tile.y+tileoffsetY)/tileH + offsetY)==1
		  || getLayer((temp_tile.x+tileoffsetX)/tileW + offsetX, (temp_tile.y+tileoffsetY)/tileH + offsetY) == 41 
		  || getLayer((temp_tile.x+tileoffsetX)/tileW + offsetX, (temp_tile.y+tileoffsetY)/tileH + offsetY) == 42)
		{
			//std::cout << "COLLISION" << std::endl;
			removeBullet(i, bullets) ; 
		}
	    else if(bullets[i]->x < 0 || bullets[i]->x > SCR_W || bullets[i]->y < 0 || bullets[i]->y > SCR_H){
			removeBullet(i, bullets);
		}
		else if(collisionAABB(boss->collider, rect)){	
			removeBullet(i, bullets) ;
			boss->health -= 1.0f ;
			std::cout << boss->health << std::endl ;
		}
		for(int j=0; j<MAX_MID; j++){
			if(collisionAABB(midgets[j]->collider, rect) && !deathM[j]){
				hurtM[j] = true ;
				removeBullet(i, bullets) ;
			}
		}
	}
	
	for(int i = 0; i < MAX_ARMS; i++) if(arms[i])
	{	
	  	arms[i]->x += arms[i]->dx ;
	  	arms[i]->y += arms[i]->dy ;
		SDL_Rect rect = { arms[i]->x, arms[i]->y, 16, 16 }; 	  
		SDL_Rect temp_tile = {arms[i]->x, arms[i]->y, 32, 32} ;	
		if(getTile((temp_tile.x+tileoffsetX)/tileW + offsetX, (temp_tile.y+tileoffsetY)/tileH + offsetY)==1
		  || getLayer((temp_tile.x+tileoffsetX)/tileW + offsetX, (temp_tile.y+tileoffsetY)/tileH + offsetY) == 41 
		  || getLayer((temp_tile.x+tileoffsetX)/tileW + offsetX, (temp_tile.y+tileoffsetY)/tileH + offsetY) == 42)
		{
			//std::cout << "COLLISION" << std::endl;
			removeBullet(i, arms) ; 
		}
	    else if(arms[i]->x < 0 || arms[i]->x > SCR_W || arms[i]->y < 0 || arms[i]->y > SCR_H){
			removeBullet(i, arms);
		}
		else if(collisionAABB(player->collider, rect)){
			removeBullet(i, arms) ;
		}
	}
	
	//Check Colliders
	if(drawColliders){
	for(int i=0; i<visTilesX; i++)
		for(int j=0; j<visTilesY; j++)
			renderer->renderRect(colliders[i][j], 128,128,128,false) ;
		
		renderer->renderRect(player->collider, 255,0,0,false) ;
		for(int i=0; i<MAX_MID; i++)
			renderer->renderRect(midgets[i]->collider, 0,255,0,false) ;
		renderer->renderRect(boss->collider, 0,0,255,false) ;		
	}
	
	renderer->display() ;
	Music::playMusic(theme, 10) ;
}

void Game::freeRsrc_(){
	renderer->freeFont(font) ;
	renderer->freeTex(playerTex) ;
	renderer->freeTex(bossTex) ;
	renderer->freeTex(midTex) ;
	renderer->freeTex(item) ;
	renderer->freeTex(temp) ;
	renderer->freeTex(laser) ;
	renderer->freeTex(bullet) ;
	renderer->freeTex(tiles) ;
	renderer->freeTex(arm_p) ;
	for(int i=0; i<2; i++)
		renderer->freeTex(tex[i]) ;
	for(int i = 0; i < MAX_BUL; i++)
    	removeBullet(i, bullets);
    for(int i = 0; i < MAX_ARMS; i++)
    	removeBullet(i, arms);
    Music::freeMusic(theme) ;
}

void Game::freeSys_(){
	delete boss ;
	delete player ;
	for(int i=0; i<MAX_MID; i++)
		delete midgets[i] ;
	Music::quit() ;
 	renderer->cleanRenderer() ;
	delete renderer ;
}
