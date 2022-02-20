
#include "../inc/Game.hpp"

int main( int argc, char* argv[] ) {

    
    Game *game = new Game();

    game->Init("LibreJump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, false);

    while( game->IsRunning() ) {
        
        game->HandleEvents();
        
    }

    game->Clean();

    return 0; 
}