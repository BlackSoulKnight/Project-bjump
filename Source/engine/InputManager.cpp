#include "InputManager.h"


map<string, bool> init() {
    map<string, bool> t;
    t["Left"] = false;
    t["Right"] = false;
    t["Jump"] = false;
    t["Dash"] = false;
    t["Drop"] = false;
    return t;
}
map<string, bool> InputManager::actionState = init();


void InputManager::pump() {
    SDL_PumpEvents();
    SDL_Event event;
    bool pressed = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            pressed = !(event.type & 1);
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_A:
                actionState["Left"] = pressed;
                break;
            case SDL_SCANCODE_D:
                actionState["Right"] = pressed;
                break;
            case SDL_SCANCODE_SPACE:
                actionState["Jump"] = pressed;
                break;
            case SDL_SCANCODE_LSHIFT:
                actionState["Dash"] = pressed;
                break;
            case SDL_SCANCODE_S:
                actionState["Drop"] = pressed;
            }

            break;
        }
    }

}