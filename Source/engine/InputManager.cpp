#include "InputManager.h"
#include "World.h"

map<string, bool> init() {
    map<string, bool> t;
    t["Left"] = false;
    t["Right"] = false;
    t["Jump"] = false;
    t["Dash"] = false;
    t["Drop"] = false;
    t["Slow"] = false;
    t["Quit"] = false;
    return t;
}

map<string, bool> InputManager::actionState = init();


void InputManager::pump() {
    SDL_PumpEvents();
    SDL_Event event;
    bool pressed = false;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            pressed = !(event.type & 1);
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                actionState["Left"] = pressed;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                actionState["Right"] = pressed;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                actionState["Jump"] = pressed;
                break;
            case SDL_SCANCODE_LSHIFT:
            case SDL_SCANCODE_RSHIFT:
                actionState["Dash"] = pressed;
                break;
            case SDL_SCANCODE_S:
                actionState["Drop"] = pressed;
                break;
            case SDL_SCANCODE_LCTRL:
                actionState["Slow"] = pressed;
                break;
            case SDL_SCANCODE_F1:
                // Hier debugConsole wert flippen true/false
                break;
            case SDL_SCANCODE_F5:
                World::Instance->loadmap(4,true);
                break;
            case SDL_SCANCODE_L:
                exit(1);
                actionState["Quit"] = pressed;
                break;
            case SDL_SCANCODE_9:
                /*string levelFile = string("../Levels/bgTest.bjump");
                World::Instance->LoadFile(levelFile, false);
                World::Instance->LoadFile(levelFile.append("BG"), true);
                World::Instance->player->setPosition(0, 0);*/
                break;
            }
            break;
            
        case SDL_QUIT:
            exit(1);
            break;
        }
    }
}