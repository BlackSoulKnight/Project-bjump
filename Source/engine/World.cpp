#include "World.h"


World::World(int _windowSizeX, int _windowSizeY) {
    windowSizeX = _windowSizeX;
    windowSizeY = _windowSizeY;
    deltaT = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(_windowSizeX, _windowSizeY, SDL_WINDOW_RESIZABLE, &window, &renderer);

}

bool World::run() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_PumpEvents();
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    return running;
}

bool World::createGameObject(GameObject* obj) {
    gameObjects.push_back(obj);
    return true;
}

bool World::destroyGameObject(GameObject* obj) {
    for (int i = 0; i < gameObjects.size(); i++) {
        if (gameObjects[i] == obj) {
            gameObjects.erase(gameObjects.begin()+i-1);
            return true;
        }
    }
    return false;
}