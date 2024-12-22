#include <SDL.h>
#include "example/mini.hpp"

extern "C"
int mainsa( int argc, char * args [] )
{
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_CreateWindowAndRenderer( 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer );
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    int i = 200;
    while( i-- ) {
        SDL_PumpEvents();
        SDL_RenderClear( renderer );
        SDL_RenderPresent( renderer );
        SDL_Delay( 16 );
    }
    return 1;
}

extern "C"
int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    NotJanSordid::SDL_MiniExample::MiniGame game;
    return game.Run();
}