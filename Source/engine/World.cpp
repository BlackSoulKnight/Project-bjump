#include "World.h"
#include <chrono>

World::World(int _windowSizeX, int _windowSizeY) {
    windowSizeX = _windowSizeX;
    windowSizeY = _windowSizeY;
    deltaT = 0;
    targetFrameRate = 60;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(_windowSizeX, _windowSizeY, SDL_WINDOW_RESIZABLE, &window, &renderer);

}

bool World::run() {
    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    if (deltaT <= 0)
        deltaT = 1;

    InputManager::pump();


    //Change WorldState
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->update(deltaT);
    }
    
    //Collide WorldState
    for (int i = 0; i < gameObjects.size(); i++) {
        gameObjects[i]->collisions = getCollisions(gameObjects[i]);
    }



    //Render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < gameObjects.size(); i++) {
        SDL_RenderFillRect(renderer, &gameObjects[i]->position);
    }
    SDL_RenderPresent(renderer);

    
    deltaT = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count();
    int frameRate = 1000 / targetFrameRate;
    if (frameRate > deltaT)
        SDL_Delay(frameRate-deltaT);
    deltaT = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count();
    return running;
}

vector<GameObject*> World::getCollisions(GameObject *origin) {
    vector<GameObject*> collisions;
    for (int i = 0; i < gameObjects.size();i++) {
        if (!gameObjects[i]->isActive || gameObjects[i] == origin)
            continue;
        if (SDL_HasIntersection(&gameObjects[i]->position, &origin->position))
            collisions.push_back(gameObjects[i]);

    }
    return collisions;
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

SDL_Renderer* World::getRenderer() {
    return renderer;
}