
/*
	Name: Shoot Em'
	Copyright: Lunatico Co.
	Author: Diwas Adhikari (Lunatico - Repo : holySchminkos)
	Date: 05/03/22 15:33
	Description: Top Down Shoot Em' Up Scroller
*/

#include "Game.hpp"

bool running = true ;
Game *game = nullptr ;

int main(int argc, char **argv)
{
	game = new Game ;
	
	game->initSys_() ;	
	game->loadRsrc_() ;
	game->init_() ;
	
	running = true ;
	
	while(running)
	{
		game->render_() ;
		running = game->update_() ;
	}
	
	game->freeRsrc_() ;
	game->freeSys_() ;
	
	delete game ;
	
	return 0 ;
}
